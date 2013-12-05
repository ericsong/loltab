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

	//extract filename
	int filenamepos = 0;
	for(int i = 0; argv[1][i] != '\0'; i++){
		if(argv[1][i] == '/')
			filenamepos = i;	
	}
	std::string filename = std::string(argv[1]).substr(filenamepos+1);
	std::string write_filename = "./scoreboards/" + filename;

	bool sb_found = false;

	int x_low = .25 * image.cols;
	int x_high = .75 * image.cols;
	int y_low = 0;
	int y_high = .3333333 * image.rows;

	//change back to lows and highs	
	for(int y = y_low; y < y_high; y++){
		int count = 0;
		double avgdiff = 0;
		for(int x = x_low; x < x_high; x++){
			int l = getl(&imglab, x, y);
			int a = geta(&imglab, x, y);
			int b = getb(&imglab, x, y);
			
			if(image.rows == 1080)	
				avgdiff += calcColorDiff(TOP_1080_l_MEAN, TOP_1080_a_MEAN, TOP_1080_b_MEAN, l, a, b);
			else if(image.rows == 720)
				avgdiff += calcColorDiff(TOP_720_l_MEAN, TOP_720_a_MEAN, TOP_720_b_MEAN, l, a, b);

			count++;	
		}
	
		//checks if top border is found	
		avgdiff = avgdiff/count;
		if(avgdiff < 20){
			sb_found = true;

			//accounts for cases where detection also picks up item shops, menus, etc
			int checklength;
			if(image.rows == 1080)
				checklength = 100;
			else if(image.rows == 720)
				checklength = 75;


			int bluecount = 0;
			int redcount = 0;	

			for(int yi = y; yi < y + checklength; yi++){
				for(int xi = x_low; xi < x_high; xi++){
					int l = getl(&imglab, xi, yi);
					int a = geta(&imglab, xi, yi);
					int b = getb(&imglab, xi, yi);
				
					double bluediff;
					double reddiff;

					if(image.rows == 1080){
						bluediff = calcColorDiff(SB_1080_BLUE_l, SB_1080_BLUE_a, SB_1080_BLUE_b, l, a, b);
						reddiff = calcColorDiff(SB_1080_RED_l, SB_1080_RED_a, SB_1080_RED_b, l, a, b);
					}else if(image.rows == 720){
						bluediff = calcColorDiff(SB_720_BLUE_l, SB_720_BLUE_a, SB_720_BLUE_b, l, a, b);
						reddiff = calcColorDiff(SB_720_RED_l, SB_720_RED_a, SB_720_RED_b, l, a, b);
					}

					if(bluediff < 20){
						bluecount++;
					}
	
					if(reddiff < 20){
						redcount++;
					}
				}
			}

			if(!(bluecount > 10 && redcount > 10))
				sb_found = false;	

			break;
		}
	}
	
	if(sb_found){
		int toppos = getScoreboardTop(&imglab, -1);	
		int leftpos = getScoreboardLeft(&imglab, -1);	
		int rightpos = getScoreboardRight(&imglab, -1);	
		int bottompos = getScoreboardBottom(&imglab, -1);	
		
		printf("%d,%d,%d,%d\n", toppos, leftpos, rightpos, bottompos);

		Rect sbregion(leftpos, toppos, rightpos - leftpos, bottompos - toppos);
		Mat sb = image(sbregion);
		imwrite(write_filename, sb);	
	}else{
		printf("scoreboard not found\n");
	}

	return 0;	
}

