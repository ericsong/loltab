#include "HistAlgos.hpp"

int *getYHist(Mat image) {
	int *hist = (int *)calloc(image.rows, sizeof(int));
	for (int y = 0; y < image.rows; y++) {
		for (int x = 0; x < image.cols; x++) {
			for (int c = 0; c < image.channels(); c++) {
				hist[y] += image.at<Vec3b>(y, x)[c];
			}
		}
	}
	return hist;
}

int *getXHist(Mat image) {
	int *hist = (int *)calloc(image.cols, sizeof(int));
	for (int y = 0; y < image.rows; y++) {
		for (int x = 0; x < image.cols; x++) {
			for (int c = 0; c < image.channels(); c++) {
				hist[x] += image.at<Vec3b>(y, x)[c];
			}
		}
	}
	return hist;
}

void plotHist(int *array, int length, char *name) {
	int max = getMax(array, length);
	Mat graph = createImage(length, length, 3);	
	float scale = divide(max, length);
	int *new_array = (int *)calloc(length, sizeof(int));
	for (int i = 0; i < length; i++) {
		new_array[i] = array[i] / scale;
	}
	for (int y = 0; y < length; y++) {
		for (int x = 0; x < length; x++) {
			for (int c = 0; c < 3; c++) {
				graph.at<Vec3b>(y, x)[c] = 0;
			}
		}
	}
	for (int x = 0; x < length; x++) {
		for (int y = 0; y < new_array[x] && y < length; y++) {
			graph.at<Vec3b>(length - 1 - y, x)[1] = 255;
		}
	}
	display(graph, name);
	graph.release();
}

void printHist(int *array, int length) {
	printf("[ ");
	for (int i = 0; i < length; i++) {
		if (i == length - 1) {
			printf("%d ]\n", array[i]);
		} else {
			printf("%d, ", array[i]);
		}
	}
}

int getMax(int *array, int length) {
	int max = 0;
	for (int i = 0; i < length; i++) {
		if (array[i] > max) {
			max = array[i];
		}
	}
	return max;
}

int *subArray(int *array, int beg, int end) {
	int *subs = (int *)calloc(end - beg, sizeof(int));
	for (int i = beg; i < end; i++) {
		subs[i - beg] = array[i];
	}
	return subs;
}

int *findLength(int *array, int length) {
	int *lengths = (int *)calloc(20, sizeof(int));
	int count = 0, index = 0;
	int max = getMax(array, length);
	int tolerance = max / 15;
	for (int i = 0; i < length; i++) {
		if (array[i] > tolerance) {
			count++;
		} else {
			if (count > 4) {
				lengths[index] = count;
				lengths[index + 10] = i - count;
				index++;
			}
			count = 0;
		}
		if (index == 10) {
			break;
		}
	}
	return lengths;
}

int *distribute(int *dataset, int size, int length) {
	int *buffer = (int *)calloc(length * 2, sizeof(int));
	int index = 0;
	for (int i = 0; i < length && index < length; i++) {
		int count = round(divide(dataset[i], size));
		for (int n = 0; n < count; n++) {
			if (index >= length) {
				break;
			}
			buffer[index] = dataset[i] / count;
			buffer[index + length] = dataset[i + length] + dataset[i] * n / count;
			index++;
		}
	}
	return buffer;
}

int *getDimensions(Mat image) {
	int *y_heights = getYHist(image);
	int *x_heights = getXHist(image);

	// calculate the sub(middleman) yaxis and xaxis, these are 20 long
	int *sub_yaxis = findLength(y_heights, image.rows);
	int *sub_xaxis = findLength(x_heights, image.cols);
	int size = getMean(subArray(sub_yaxis, 0, 10), 10);
	sub_yaxis = distribute(sub_yaxis, size, 10);
	sub_xaxis = distribute(sub_xaxis, size, 10);
	
	// normalize the yaxis
	int *startpoints = (int *)calloc(30, sizeof(int));
	for (int i = 0; i < 10; i++) {
		int midPoint = size / 2 + sub_yaxis[i + 10];
		int start = midPoint;
		int end = midPoint;
		for (int cap = 0; cap < size; cap++) {
			if (y_heights[start - 1] > y_heights[end]) {
				start--;
			} else {
				end++;
			}
		}
		startpoints[i] = start;// + 1;
	}

	// normalize the xaxis
	for (int i = 0; i < 9; i++) {
		int midPoint = size / 2 + sub_xaxis[i + 10];
		int start = midPoint;
		int end = midPoint;
		for (int cap = 0; cap < size; cap++) {
			if (x_heights[start - 1] > x_heights[end]) {
				start--;
			} else {
				end++;
			}
		}
		startpoints[10 + i] = start;// + 1;
	}

	startpoints[19] = size;// - 2;
	int fadePoint = getMax(y_heights, image.rows) / 2; // this might cause some errors, probably have to change in the future
	for (int i = 0; i < 10; i++) {
		startpoints[i + 20] = getMean(subArray(y_heights, startpoints[i], startpoints[i] + size), size) > fadePoint;
	}

	return startpoints;
}

char *getExpression(Mat image, int faded) {
	//obtain the y1 and y2 of the image
	int *yhist = getYHist(image);
	int peakAverage = ceiling(divide(getSum(yhist, image.rows), image.rows));
	for (int i = 0; i < image.rows; i++) {
		yhist[i] -= peakAverage;
		if (yhist[i] < 0) {
			yhist[i] = 0;
		}
	}
	int start_y = 0;
	int length_y = 0;
	for (int y = 0; y < image.rows; y++) {
		if (yhist[y] != 0) {
			length_y++;
		} else if (length_y != 0) {
			start_y = y - length_y;
			break;
		}
	}
	if (length_y == 0) {
		return NULL;
	}

	int *xhist = getXHist(image);
	int totalAverage = getMean(xhist, image.cols);
	for (int i = 0; i < image.cols; i++) {
		xhist[i] = xhist[i] - totalAverage;
		if (xhist[i] < 0) {
			xhist[i] = 0;
		}
	}

	char *buffer = (char *)calloc(24, sizeof(char));
	int buffer_n = 0;
	// gather all the data points
	int *startpoints = (int *)calloc(24, sizeof(int));
	int deltawye = 0;
	int *pointlengths = (int *)calloc(24, sizeof(int));
	int index = 0;
	for (int x = 0; x < image.cols; x++) {
		if (xhist[x] != 0) {
			deltawye++;
		} else if (deltawye != 0) {
			startpoints[index] = x - deltawye;
			pointlengths[index] = deltawye;
			deltawye = 0;
			Mat test_image = subImage(image, startpoints[index], start_y, x, start_y + length_y);
			char c = matchImage(test_image, faded);
			if (buffer_n == 0 || c != '/' || buffer[buffer_n - 1] != '/') {
				buffer[buffer_n++] = c;
			}
			index++;
		}
	}
	buffer[buffer_n] = '\0';

	return buffer;
}

char matchImage(Mat image, int faded) {
	double error = 1.0;
	char c = 12;
	for (int i = 0; i < 11; i++) {
		if (faded) {
			for (int n = 0; n < FADED_CHARACTERS_LENGTH[i]; n++) {
				Mat compareToImage = FADED_CHARACTERS[i][n];
				compareToImage = resizeImage(compareToImage, image.cols, image.rows);
				double interim_error = compareImage(compareToImage, image);
				if (interim_error <= error) {
					error = interim_error;
					c = i;
				}
			}
		} else {
			for (int n = 0; n < CHARACTERS_LENGTH[i]; n++) {
				Mat compareToImage = CHARACTERS[i][n];
				compareToImage = resizeImage(compareToImage, image.cols, image.rows);
				double interim_error = compareImage(compareToImage, image);
				if (interim_error <= error) {
					error = interim_error;
					c = i;
				}
			}
		}
	}
	if (c < 10) {
		c += '0';
	} else if (c == 10) {
		c = '/';
	} else {
		c = '?';
	}
	return c;
}

int getMean(int *array, int length) {
	return round(divide(getSum(array, length), length));
}

int ceiling(float x) {
	return (x - (int)x == 0.0) ? ((int)x) : ((int)x + 1);
}

int getSum(int *array, int length) {
	int total = 0;
	for (int i = 0; i < length; i++) {
		total += array[i];
	}
	return total;
}

float multiply(float x, float y) {
	return x * y;
}

float divide(float x, float y) {
	return x / y;
}

int round(float x) {
	return (x - (int)x >= 0.5) ? ((int)x + 1) : ((int)x);
}
