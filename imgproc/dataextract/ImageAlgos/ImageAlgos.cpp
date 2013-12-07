#include "ImageAlgos.hpp"

void display(Mat image) {
	namedWindow("asdf", CV_WINDOW_AUTOSIZE);
	imshow("asdf", image);
	waitKey(0);
}

Mat createImage(int width, int height, int channels) {
	Mat new_image;
	new_image.create(height, width, CV_8UC(channels));
	return new_image;
}

Mat subImage(Mat image, int x1, int y1, int x2, int y2) {
	int dx = x2 - x1;
	int dy = y2 - y1;
	if (dx <= 0 || dy <= 0 || y2 > image.rows || x2 > image.cols) {
		return image;
	}
	Mat new_image = createImage(dx, dy, image.channels());
	for (int y = y1; y < y2; y++) {
		for (int x = x1; x < x2; x++) {
			for (int c = 0; c < image.channels(); c++) {
				new_image.at<Vec3b>(y - y1, x - x1)[c] =
					image.at<Vec3b>(y, x)[c];
			}
		}
	}
	return new_image;
}

Mat resizeImage(Mat image, int width, int height) {
	Mat new_image = createImage(width, height, image.channels());
	float xscale = (((float)width) / ((float)image.cols));
	float yscale = (((float)height) / ((float)image.rows));
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int c = 0; c < image.channels(); c++) {
				new_image.at<Vec3b>(y, x)[c] = image.at<Vec3b>(
					(int)(((float)y) / yscale),
					(int)(((float)x) / xscale))[c];
			}
		}
	}
	return new_image;
}

Mat recolorImage(Mat image, int brightness, float contrast) {
	int changed[256];
	for (int i = 0; i < 256; i++) {
		changed[i] = saturate_cast<unsigned char>(contrast * i + brightness);
		if (changed[i] <= 0) {
			changed[i] = 0;
		} else if (changed[i] > 255) {
			changed[i] = 255;
		}
	}
	unsigned char lookup[256];
	for (int i = 0; i < 256; i++) {
		lookup[i] = changed[i];
	}
	Mat new_image = createImage(image.cols, image.rows, image.channels());
	for (int y = 0; y < image.rows; y++) {
		for (int x = 0; x < image.cols; x++) {
			for (int c = 0; c < image.channels(); c++) {
				new_image.at<Vec3b>(y, x)[c] = lookup[image.at<Vec3b>(y, x)[c]];
			}
		}
	}
	return new_image;
}

double compareImage(Mat image1, Mat image2) {
	double error = 0.00, max_error = image1.rows * image1.cols * 255.0 * 255.0 * image1.channels();
	for (int y = 0; y < image1.rows; y++) {
		for (int x = 0; x < image1.cols; x++) {
			for (int c = 0; c < image1.channels(); c++) {
				int difference = image1.at<Vec3b>(y, x)[c] - image2.at<Vec3b>(y, x)[c];
				error += (double)(difference * difference);
			}
		}
	}
	return error / max_error;
}

Mat extremifyImage(Mat image, int tolerance) {
	unsigned char lookup[256];
	for (int i = 0; i < tolerance; i++) {
		lookup[i] = 0;
	}
	for (int i = tolerance; i < 256; i++) {
		lookup[i] = 255;
	}
	Mat new_image = createImage(image.cols, image.rows, image.channels());
	for (int y = 0; y < image.rows; y++) {
		for (int x = 0; x < image.cols; x++) {
			for (int c = 0; c < image.channels(); c++) {
				new_image.at<Vec3b>(y, x)[c] = lookup[image.at<Vec3b>(y, x)[c]];
			}
		}
	}
	return new_image;
}

int averageColor(Mat image) {
	unsigned int color = 0;
	for (int y = 0; y < image.rows; y++) {
		for (int x = 0; x < image.cols; x++) {
			for (int c = 0; c < image.channels(); c++) {
				color += image.at<Vec3b>(y, x)[c];
			}
		}
	}
	return color / (image.rows * image.cols);
}

void blit(Mat screen, Mat image, int x_offset, int y_offset) {
	int rows = image.rows;
	if (rows + y_offset > screen.rows) {
		rows = screen.rows - y_offset;
	}
	int cols = image.cols;
	if (cols + x_offset > screen.cols) {
		cols = screen.cols - x_offset;
	}
	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {
			for (int c = 0; c < image.channels(); c++) {
				screen.at<Vec3b>(y + y_offset, x + x_offset)[c] = image.at<Vec3b>(y, x)[c];
			}
		}
	}
}

char isValidForMatch(char c) {
	return (c == '/') || ('0' <= c && c <= '9');
}

int *getBlob(Mat image, int x, int y, char **visited) {
	ArrayList *X = CreateList();
	ArrayList *Y = CreateList();
	ALInsert(X, x);
	ALInsert(Y, y);
	int length = 0, lx = x, ly = y, hx = x, hy = y;
	visited[y][x] = 1;
	while (length < X->length) {
		// add to paired list
		x = ALGet(X, length);
		y = ALGet(Y, length);
		if (image.at<Vec3b>(y, x)[0] == 255 ||
			image.at<Vec3b>(y, x)[1] == 255 ||
			image.at<Vec3b>(y, x)[2] == 255) {
			if (((x + 1) < image.cols) && !(visited[y][x + 1])) {
				ALInsert(X, x + 1);
				ALInsert(Y, y);
				visited[y][x + 1] = 1;
			}
			if (((y + 1) < image.rows) && !(visited[y + 1][x])) {
				ALInsert(X, x);
				ALInsert(Y, y + 1);
				visited[y + 1][x] = 1;
			}
			if (((x - 1) >= 0) && !(visited[y][x - 1])) {
				ALInsert(X, x - 1);
				ALInsert(Y, y);
				visited[y][x - 1] = 1;
			}
			if (((y - 1) >= 0) && !(visited[y - 1][x])) {
				ALInsert(X, x);
				ALInsert(Y, y - 1);
				visited[y - 1][x] = 1;
			}
			if (((x + 1) < image.cols) && ((y - 1) >= 0) && !(visited[y - 1][x + 1])) {
				ALInsert(X, x + 1);
				ALInsert(Y, y - 1);
				visited[y - 1][x + 1] = 1;
			}
			if (((x - 1) >= 0) && ((y + 1) < image.rows) && !(visited[y + 1][x - 1])) {
				ALInsert(X, x - 1);
				ALInsert(Y, y + 1);
				visited[y + 1][x - 1] = 1;
			}
			if (x < lx) {
				lx = x;
			} else if (x > hx) {
				hx = x;
			}
			if (y < ly) {
				ly = y;
			} else if (y > hy) {
				hy = y;
			}
		}
		length++;
	}
	int *coords = (int *)calloc(4, sizeof(int));
	coords[0] = lx;
	coords[1] = ly;
	coords[2] = hx + 1;
	coords[3] = hy + 1;
	DestroyList(X);
	DestroyList(Y);
	return coords;
}
