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
		avgdiff = avgdiff/count;
		
		if(avgdiff < 20)
			printf("scoreboard top border found at  %d\n", y);

	}

	return 0;	
}

