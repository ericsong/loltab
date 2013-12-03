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

/*
#define TOP_RED_LOW 142
#define TOP_RED_HIGH 164
#define TOP_GREEN_LOW 148
#define TOP_GREEN_HIGH 160
#define TOP_BLUE_LOW 137
#define TOP_BLUE_HIGH 165
*/

#define TOP_RED_LOW 122
#define TOP_RED_HIGH 184
#define TOP_GREEN_LOW 128
#define TOP_GREEN_HIGH 180
#define TOP_BLUE_LOW 137
#define TOP_BLUE_HIGH 185


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

int main(int argc, char* argv[]) 
{
	Mat image = imread(argv[1], 1);

	int x_low = .25 * image.cols;
	int x_high = .75 * image.cols;
	int y_low = 0;
	int y_high = .3333333 * image.rows;
	
	for(int y = y_low; y < y_high; y++){
		int count = 0;
		for(int x = x_low; x < x_high; x++){
			int r = getRed(&image, x, y);
			int g = getGreen(&image, x, y);
			int b = getBlue(&image, x, y);

			if( (r > TOP_RED_LOW && r < TOP_RED_HIGH) && (b > TOP_BLUE_LOW && b < TOP_BLUE_HIGH) && (g > TOP_GREEN_LOW && g < TOP_GREEN_HIGH) )
				count++;
		}
	
		printf("%d: %d\n", y, count);

		if(count > ((x_high-x_low)*.5) ){
			//at this point the image is very likely to be a scoreboard or an item shop
			//following code checks if its a scoreboard if an item shop or sb is passed in
			
			int BLUE_R = 44;
			int BLUE_G = 214;
			int BLUE_B = 211;	

			int RED_R = 194;
			int RED_G = 5;
			int RED_B = 44;	

			int DIFF = 20;

			int bluecount = 0;
			int redcount = 0;

			for(int y_i = y; y_i < y + (image.rows*.1); y_i++){
				for(int x_i = x_low; x_i < x_high; x_i++){
					int r = getRed(&image, x_i, y_i);
					int g = getGreen(&image, x_i, y_i);
					int b = getBlue(&image, x_i, y_i);

					if( (r < BLUE_R + DIFF && r > BLUE_R - DIFF) && (g < BLUE_G + DIFF && g > BLUE_G - DIFF) && (b < BLUE_B + DIFF && b > BLUE_B - DIFF) )
						bluecount++;

					if( (r < RED_R + DIFF && r > RED_R - DIFF) && (g < RED_G + DIFF && g > RED_G - DIFF) && (b < RED_B + DIFF && b > RED_B - DIFF) )
						redcount++;

				}
			}

			if(bluecount > 10 && redcount > 10){
				printf("top bar:  %d\n", y);
				printf("left bar: %d\n", getScoreboardLeft(&image));
				printf("right bar: %d\n", getScoreboardRight(&image));
				printf("bottom bar: %d\n", getScoreboardBottom(&image));
			}
		}
	}

	return 0;	
}

