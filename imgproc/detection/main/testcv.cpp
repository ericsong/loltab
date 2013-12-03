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

	int lab_l_sum = 0;
	int lab_a_sum = 0;
	int lab_b_sum = 0;
	
	int rgb_r_sum = 0;
	int rgb_g_sum = 0;
	int rgb_b_sum = 0;

	int count = 0;

	for(int y = 205; y < 206; y++){
		for(int x = 560; x < 1400; x++){
			int L = imglab.at<Vec3b>(y,x)[0];	
			int a = imglab.at<Vec3b>(y,x)[1];	
			int b = imglab.at<Vec3b>(y,x)[2];
	
			int R = image.at<Vec3b>(y,x)[2];	
			int G = image.at<Vec3b>(y,x)[1];	
			int B = image.at<Vec3b>(y,x)[0];
	
			lab_l_sum += L;
			lab_a_sum += a;
			lab_b_sum += b;
			
			rgb_r_sum += R;
			rgb_g_sum += G;
			rgb_b_sum += B;
	
			count++;	
		}
	}

	printf("RGB averages: %f, %f, %f\n", (double) rgb_r_sum/count, (double) rgb_r_sum/count, (double) rgb_b_sum/count);
	printf("Lab averages: %f, %f, %f\n", (double) lab_l_sum/count / 255 * 100, (double) lab_a_sum/count -128 , (double) lab_b_sum/count - 128);
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

