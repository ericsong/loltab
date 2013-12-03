#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include "sbcolors.h"
#include "sb_imglib.h"

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

int getl(Mat *image, int x, int y){
	return image->at<Vec3b>(y, x)[0];
}

int geta(Mat *image, int x, int y){
	return image->at<Vec3b>(y, x)[1];
}

int getb(Mat *image, int x, int y){
	return image->at<Vec3b>(y, x)[2];
}

int getScoreboardLeft(Mat *image, int guess = -1){
	int x_low = 0;
	int x_high = .5 * image->cols;
	int y_low = .2 * image->rows;
	int y_high = .8 * image->rows;

	int highest_count = -1;
	int pos;

	for(int x = x_low; x < x_high; x++){
		int count = 0;
		for(int y = y_low; y < y_high; y++){
			int r = getRed(image, x, y);
			int g = getGreen(image, x, y);
			int b = getBlue(image, x, y);

			if( (r > LEFT_RED_LOW && r < LEFT_RED_HIGH) && (b > LEFT_BLUE_LOW && b < LEFT_BLUE_HIGH) && (g > LEFT_GREEN_LOW && g < LEFT_GREEN_HIGH) )
				count++;
		}

		if(count > highest_count){
			highest_count = count;
			pos = x;	
		}
	}	

	return pos;
}

int getScoreboardTop(Mat *image, int guess = -1){
	int x_low = 0;
	int x_high = .5 * image->cols;
	int y_low = .2 * image->rows;
	int y_high = .8 * image->rows;

	int highest_count = -1;
	int leftpos;

	for(int x = x_low; x < x_high; x++){
		int count = 0;
		for(int y = y_low; y < y_high; y++){
			int r = getRed(image, x, y);
			int g = getGreen(image, x, y);
			int b = getBlue(image, x, y);

			if( (r > LEFT_RED_LOW && r < LEFT_RED_HIGH) && (b > LEFT_BLUE_LOW && b < LEFT_BLUE_HIGH) && (g > LEFT_GREEN_LOW && g < LEFT_GREEN_HIGH) )
				count++;
		}

		if(count > highest_count){
			highest_count = count;
			leftpos = x;	
		}
	}	

	return leftpos;
}

int getScoreboardRight(Mat *image, int guess = -1){
	int x_low = .5 * image->cols;
	int x_high = image->cols;
	int y_low = .2 * image->rows;
	int y_high = .8 * image->rows;

	int highest_count = -1;
	int pos;

	for(int x = x_low; x < x_high; x++){
		int count = 0;
		for(int y = y_low; y < y_high; y++){
			int r = getRed(image, x, y);
			int g = getGreen(image, x, y);
			int b = getBlue(image, x, y);

			if( (r > RIGHT_RED_LOW && r < RIGHT_RED_HIGH) && (b > RIGHT_BLUE_LOW && b < RIGHT_BLUE_HIGH) && (g > RIGHT_GREEN_LOW && g < RIGHT_GREEN_HIGH) )
				count++;
		}

		if(count > highest_count){
			highest_count = count;
			pos = x;	
		}
	}	

	return pos;
}

int getScoreboardBottom(Mat *image, int guess = -1){
	int x_low = .25 * image->cols;
	int x_high = .75 * image->cols;
	int y_low = .5 * image->rows;
	int y_high = image->rows;

	int highest_count = -1;
	int pos;

	for(int y = y_low; y < y_high; y++){
		int count = 0;
		for(int x = x_low; x < x_high; x++){
			int r = getRed(image, x, y);
			int g = getGreen(image, x, y);
			int b = getBlue(image, x, y);

			if( (r > BOTTOM_RED_LOW && r < BOTTOM_RED_HIGH) && (b > BOTTOM_BLUE_LOW && b < BOTTOM_BLUE_HIGH) && (g > BOTTOM_GREEN_LOW && g < BOTTOM_GREEN_HIGH) )
				count++;
		}

		if(count > highest_count){
			highest_count = count;
			pos = y;	
		}
	}	

	return pos;
}


