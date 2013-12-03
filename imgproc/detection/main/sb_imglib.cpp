#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <math.h>
#include "sbcolors.h"
#include "sb_imglib.h"

using namespace cv;

double calcColorDiff(int a1, int b1, int c1, int a2, int b2, int c2){
	return sqrt((a1-a2)*(a1-a2) + (b1-b2)*(b1-b2) + (c1-c2)*(c1-c2));	
}

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

bool isTopBorder(int l, int a, int b, int resolution){
	//different thresholds used for 720p and 1080p
	double diff; 
	
	if(resolution == 720)
		diff = calcColorDiff(TOP_720_l_MEAN, TOP_720_a_MEAN, TOP_720_b_MEAN, l, a, b);
	else if(resolution == 1080)
		diff = calcColorDiff(TOP_1080_l_MEAN, TOP_1080_a_MEAN, TOP_1080_b_MEAN, l, a, b);

	if(diff < 20)
		return true;
	else
		return false;
/*
 * comparison version
	if(resolution == 720){
		if(	(l > TOP_720_l_MEAN - TOP_720_l_DIFF) &&
			(l < TOP_720_l_MEAN + TOP_720_l_DIFF) &&
			(a > TOP_720_a_MEAN - TOP_720_a_DIFF) &&
			(a < TOP_720_a_MEAN + TOP_720_a_DIFF) &&
			(b > TOP_720_b_MEAN - TOP_720_b_DIFF) &&
			(b < TOP_720_b_MEAN + TOP_720_b_DIFF) )
		{
			return true;	
		}
	}else if(resolution == 1080){
		if(	(l > TOP_1080_l_MEAN - TOP_1080_l_DIFF) &&
			(l < TOP_1080_l_MEAN + TOP_1080_l_DIFF) &&
			(a > TOP_1080_a_MEAN - TOP_1080_a_DIFF) &&
			(a < TOP_1080_a_MEAN + TOP_1080_a_DIFF) &&
			(b > TOP_1080_b_MEAN - TOP_1080_b_DIFF) &&
			(b < TOP_1080_b_MEAN + TOP_1080_b_DIFF) )
		{
			return true;	
		}
	}

	return false;

*/
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


