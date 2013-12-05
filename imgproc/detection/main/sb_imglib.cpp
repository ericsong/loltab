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

	int pos = 10;
	double lowest_val = 999999999;

	for(int x = x_low; x < x_high; x++){
		int count = 0;
		double avgdiff = 0;

		for(int y = y_low; y < y_high; y++){
			int l = getl(image, x, y);
			int a = geta(image, x, y);
			int b = getb(image, x, y);

                        if(image->rows == 1080)
				avgdiff += calcColorDiff(LEFT_1080_l_MEAN, LEFT_1080_a_MEAN, LEFT_1080_b_MEAN, l, a, b);
                        else if(image->rows == 720)
                                avgdiff += calcColorDiff(LEFT_720_l_MEAN, LEFT_720_a_MEAN, LEFT_720_b_MEAN, l, a, b);

			count++;
		}
		
		avgdiff = avgdiff/count;

		if(avgdiff < lowest_val){
			lowest_val = avgdiff;
			pos = x;
		}
	}	

	return pos;
}

int getScoreboardTop(Mat *image, int guess = -1){
	int x_low = .25 * image->cols;
	int x_high = .75 * image->cols;
	int y_low = 0;
	int y_high = .5 * image->rows;

	int pos;

	double lowest_val = 999999999;

	for(int y = y_low; y < y_high; y++){
		int count = 0;
		double avgdiff = 0;

		for(int x = x_low; x < x_high; x++){
			int l = getl(image, x, y);
			int a = geta(image, x, y);
			int b = getb(image, x, y);

                        if(image->rows == 1080)
				avgdiff += calcColorDiff(TOP_1080_l_MEAN, TOP_1080_a_MEAN, TOP_1080_b_MEAN, l, a, b);
                        else if(image->rows == 720)
                                avgdiff += calcColorDiff(TOP_720_l_MEAN, TOP_720_a_MEAN, TOP_720_b_MEAN, l, a, b);

			count++;
		}
		
		avgdiff = avgdiff/count;
	
		if(avgdiff < lowest_val){
			lowest_val = avgdiff;
			pos = y;
		}	
	}	

	return pos;
}

int getScoreboardRight(Mat *image, int guess = -1){
	int x_low = .5 * image->cols;
	int x_high = image->cols;
	int y_low = .2 * image->rows;
	int y_high = .8 * image->rows;

	int pos;
	double lowest_val = 999999999;
	
	for(int x = x_low; x < x_high; x++){
		int count = 0;
		double avgdiff = 0;

		for(int y = y_low; y < y_high; y++){
			int l = getl(image, x, y);
			int a = geta(image, x, y);
			int b = getb(image, x, y);

                        if(image->rows == 1080)
				avgdiff += calcColorDiff(RIGHT_1080_l_MEAN, RIGHT_1080_a_MEAN, RIGHT_1080_b_MEAN, l, a, b);
                        else if(image->rows == 720)
                                avgdiff += calcColorDiff(RIGHT_720_l_MEAN, RIGHT_720_a_MEAN, RIGHT_720_b_MEAN, l, a, b);

			count++;
		}
		
		avgdiff = avgdiff/count;

		if(avgdiff < lowest_val){
			lowest_val = avgdiff;
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

	int pos;

	double lowest_val = 999999999;

	for(int y = y_low; y < y_high; y++){
		int count = 0;
		double avgdiff = 0;

		for(int x = x_low; x < x_high; x++){
			int l = getl(image, x, y);
			int a = geta(image, x, y);
			int b = getb(image, x, y);

                        if(image->rows == 1080)
				avgdiff += calcColorDiff(BOTTOM_1080_l_MEAN, BOTTOM_1080_a_MEAN, BOTTOM_1080_b_MEAN, l, a, b);
                        else if(image->rows == 720)
                                avgdiff += calcColorDiff(BOTTOM_720_l_MEAN, BOTTOM_720_a_MEAN, BOTTOM_720_b_MEAN, l, a, b);

			count++;
		}
		
		avgdiff = avgdiff/count;
	
		if(avgdiff < lowest_val){
			lowest_val = avgdiff;
			pos = y;
		}	
	}	

	return pos;
}


