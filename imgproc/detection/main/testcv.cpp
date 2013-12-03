#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>

using namespace cv;

int getRed(Mat *image, int x, int y){
	return image->at<Vec3b>(y, x)[2];
}

int getGreen(Mat *image, int x, int y){
	return image->at<Vec3b>(y, x)[1];
}

int getBlue(Mat *image, int x, int y){
	return image->at<Vec3b>(y, x)[0];
}

int main(int argc, char* argv[]) 
{
	Mat image = imread("../img/dyrus_1080p/img1030.jpg", 1);
	Mat imglab;
	cvtColor(image, imglab, CV_BGR2Lab);

	int highest_r = -1;
	int lowest_r = 1000;
	int highest_g = -1;
	int lowest_g = 1000;
	int highest_b = -1;
	int lowest_b = 1000;

	for(int y = 0; y < 1080; y++){
		for(int x = 0; x < 1920; x++){
			double L = (double) imglab.at<Vec3b>(y,x)[0] / 255 * 100;	
			double a = imglab.at<Vec3b>(y,x)[1]-128;	
			double b = imglab.at<Vec3b>(y,x)[2]-128;
	
			int R = image.at<Vec3b>(y,x)[2];	
			int G = image.at<Vec3b>(y,x)[1];	
			int B = image.at<Vec3b>(y,x)[0];
	
	
			printf("(%d,%d)\n", x, y); 
			printf("Lab: %f, %f, %f\n", L, a, b);	
			printf("rgb: %d, %d, %d\n", R, G, B);	
		}
	}
/*
	printf("Average Red: %f\n", (double)rsum/count);
	printf("Average Green: %f\n", (double)gsum/count);
	printf("Average Blue: %f\n", (double)bsum/count);

	printf("Low/High red: %d, %d\n", lowest_r, highest_r);
	printf("Low/High green: %d, %d\n", lowest_g, highest_g);
	printf("Low/High blue: %d, %d\n", lowest_b, highest_b);
*/	
	return 0;	
}

