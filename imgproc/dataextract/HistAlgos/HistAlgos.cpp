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

void plotHist(int *array, int length, int scale) {
	Mat graph = createImage(length, 500, 3);
	for (int x = 0; x < length; x++) {
		for (int y = 0; y < array[x] / scale && y < 500; y++) {
			graph.at<Vec3b>(499 - y, x)[0] = 255;
		}
	}
	imwrite("lala2.png", graph);
	display(graph);
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

int *findAverages(int *array, int length) {
	int *avgs = (int *)calloc(10, sizeof(int));
	int count = 0, index = 0;
	int max = getMax(array, length);
	int tolerance = max / 10;
	for (int i = 0; i < length; i++) {
		if (array[i] >= tolerance) {
			count++;
		} else {
			if (count > 4) {
				avgs[index++] = count;
				count = 0;
			}
		}
		if (index == 10) {
			break;
		}
	}
	printf("[");
	for (int i = 0; i < 10; i++) {
		printf("%d, ", avgs[i]);
	}
	printf("]\n");
	return avgs;
}

int mean(int *array) {
	int numItems = 0;
	int total = 0;
	for (int i = 0; i < 10; i++) {
		if (array[i] != 0) {
			total += array[i];
			numItems++;
		}
	}
	return DIV(total, numItems);
}

int MUL(float x, float y) {
	float ans = (x * y);
	if ((int)(ans * 10.0) % 10 >= 5) {
		ans += 1.0;
	}
	return (int)ans;
}

int DIV(float x, float y) {
	float ans = (x / y);
	if ((int)(ans * 10.0) % 10 >= 5) {
		ans += 1.0;
	}
	return (int)ans;
}
