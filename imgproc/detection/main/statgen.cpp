#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>

#define LEFT_RED_LOW 214
#define LEFT_RED_HIGH 243
#define LEFT_GREEN_LOW 181 
#define LEFT_GREEN_HIGH 194
#define LEFT_BLUE_LOW 128
#define LEFT_BLUE_HIGH 145

#define TOP_RED_LOW 142
#define TOP_RED_HIGH 164
#define TOP_GREEN_LOW 148
#define TOP_GREEN_HIGH 160
#define TOP_BLUE_LOW 137
#define TOP_BLUE_HIGH 165

#define RIGHT_RED_LOW 178
#define RIGHT_RED_HIGH 218
#define RIGHT_GREEN_LOW 167
#define RIGHT_GREEN_HIGH 206
#define RIGHT_BLUE_LOW 105
#define RIGHT_BLUE_HIGH 148

#define BOTTOM_RED_LOW 171
#define BOTTOM_RED_HIGH 196
#define BOTTOM_GREEN_LOW 175
#define BOTTOM_GREEN_HIGH 200
#define BOTTOM_BLUE_LOW 172
#define BOTTOM_BLUE_HIGH 201

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
	int x_low = atoi(argv[1]); 
	int x_high = atoi(argv[2]);
	int y_low = atoi(argv[3]); 
	int y_high = atoi(argv[4]); 

	Mat image = imread("../img/img1030.jpg", 1);

	int rsum = 0;
	int gsum = 0;
	int bsum = 0;
	int count = 0;

	int highest_r = -1;
	int lowest_r = 1000;
	int highest_g = -1;
	int lowest_g = 1000;
	int highest_b = -1;
	int lowest_b = 1000;

	for(int y = y_low; y < y_high; y++){
		for(int x = x_low; x < x_high; x++){
			int r = getRed(&image, x, y);
			int g = getGreen(&image, x, y);
			int b = getBlue(&image, x, y);

			rsum += r;
			gsum += g;
			bsum += b;
			count++;

			if(r > highest_r)
				highest_r = r;
			if(r < lowest_r)
				lowest_r = r;

			if(g > highest_g)
				highest_g = g;
			if(g < lowest_g)
				lowest_g = g;

			if(b > highest_b)
				highest_b = b;
			if(b < lowest_b)
				lowest_b = b;
		}
	}

	printf("Average Red: %f\n", (double)rsum/count);
	printf("Average Green: %f\n", (double)gsum/count);
	printf("Average Blue: %f\n", (double)bsum/count);

	printf("Low/High red: %d, %d\n", lowest_r, highest_r);
	printf("Low/High green: %d, %d\n", lowest_g, highest_g);
	printf("Low/High blue: %d, %d\n", lowest_b, highest_b);
	
	return 0;	
}

