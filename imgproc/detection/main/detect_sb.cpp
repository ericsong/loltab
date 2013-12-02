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
	int x_low = 500;
	int x_high = 1500;
	int y_low = 100;
	int y_high = 300;
	
	Mat image = imread(argv[1], 1);

	for(int y = y_low; y < y_high; y++){
		int count = 0;
		for(int x = x_low; x < x_high; x++){
			int r = getRed(&image, x, y);
			int g = getGreen(&image, x, y);
			int b = getBlue(&image, x, y);

			if( (r > TOP_RED_LOW && r < TOP_RED_HIGH) && (b > TOP_BLUE_LOW && b < TOP_BLUE_HIGH) && (g > TOP_GREEN_LOW && g < TOP_GREEN_HIGH) )
				count++;
		}
		
		if(count > ((x_high-x_low)*.5) )
			printf("scoreboard detected\n");
	}

	return 0;	
}

