#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include "sbcolors.h"
#include "sb_imglib.h"

using namespace cv;

int main(int argc, char* argv[]) 
{
	Mat image = imread(argv[1], 1);
	Mat imglab;
	cvtColor(image, imglab, CV_BGR2Lab);

	int x_low = .25 * image.cols;
	int x_high = .75 * image.cols;
	int y_low = 0;
	int y_high = .3333333 * image.rows;
	
	for(int y = y_low; y < y_high; y++){
		int count = 0;
		for(int x = x_low; x < x_high; x++){
			int l = getl(&imglab, x, y);
			int a = geta(&imglab, x, y);
			int b = getb(&imglab, x, y);
			
			if( (l > TOP_l_MEAN - TOP_l_DIFF && l < TOP_l_MEAN + TOP_l_DIFF) &&  (a > TOP_a_MEAN - TOP_a_DIFF && a < TOP_a_MEAN + TOP_a_DIFF) &&  (b > TOP_b_MEAN - TOP_b_DIFF && b < TOP_b_MEAN + TOP_b_DIFF) ){
				count++;
			}


		}

		if(count > ((x_high - x_low)*.5))	
			printf("%d: %d\n", y, count);

		/*
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
		*/
	}

	return 0;	
}

