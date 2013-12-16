#include <stdio.h>
#include <stdlib.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int abs(int x) {
	return x < 0 ? -x : x;
}

int main(int argc, char **argv) {
	char buffer[100];
	if (argc != 2) {
		fprintf(stderr, "Incorrect usage. Proper usage: ./alert <image_name>.\n");
		return 1;
	}
	Mat image = imread(argv[1], 1);
	if (!image.data) {
		fprintf(stderr, "No image here.\n");
		return 1;
	}
	
	// get three lines
	int line1 = image.rows / 4;
	int line2 = image.rows / 2;
	int line3 = image.rows * 3 / 4;
	int isBlack1[image.cols];
	int isBlack2[image.cols];
	int isBlack3[image.cols];

	for (int x = 0; x < image.cols; x++) {
		isBlack1[x] =	image.at<Vec3b>(line1, x)[0] == 0 &&
						image.at<Vec3b>(line1, x)[1] == 0 &&
						image.at<Vec3b>(line1, x)[2] == 0;
		isBlack2[x] =	image.at<Vec3b>(line2, x)[0] == 0 &&
						image.at<Vec3b>(line2, x)[1] == 0 &&
						image.at<Vec3b>(line2, x)[2] == 0;
		isBlack3[x] =	image.at<Vec3b>(line3, x)[0] == 0 &&
						image.at<Vec3b>(line3, x)[1] == 0 &&
						image.at<Vec3b>(line3, x)[2] == 0;
	}

	int left1 = 0, right1 = image.cols;
	int left2 = 0, right2 = image.cols;
	int left3 = 0, right3 = image.cols;
	while (isBlack1[left1]) {
		left1++;
	}
	while (right1 > left1 && isBlack1[right1 - 1]) {
		right1--;
	}
	while (isBlack2[left2]) {
		left2++;
	}
	while (right2 > left2 && isBlack2[right2 - 1]) {
		right2--;
	}
	while (isBlack3[left3]) {
		left3++;
	}
	while (right3 > left3 && isBlack3[right3 - 1]) {
		right3--;
	}

	int width1 = right1 - left1;
	int width2 = right2 - left2;
	if (width2 == 0) {
		puts("false");
		return 0;
	}

	if (abs(left1 - left3) < 5 && abs(right1 - right3) < 5) {
		float error = ((float)abs(width1 / 5 - width2)) / ((float)width2);
		if (width2 > 20 && error < 0.23 && error > 0.20) {
			int passed = 1;
			/*int delta = (width1 * 1.005) / 5;
			for (int i = 0; i < 4; i++) {
				if (!isBlack2[left2 + delta * i]) {
					passed = 0;
					break;
				}
			}*/
			if (passed) {
				puts("true");
				return 0;
			}
		}
	}
	puts("false");
	return 0;
}
