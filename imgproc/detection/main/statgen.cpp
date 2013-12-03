#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <math.h>

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

int getL(Mat *image, int x, int y){
	return image->at<Vec3b>(y, x)[0];
}

int getA(Mat *image, int x, int y){
	return image->at<Vec3b>(y, x)[1];
}

int getB(Mat *image, int x, int y){
	return image->at<Vec3b>(y, x)[2];
}

int main(int argc, char* argv[]) 
{
	Mat image = imread(argv[1], 1);
	Mat imglab;
	cvtColor(image, imglab, CV_BGR2Lab);
	
	int x_low = atoi(argv[2]); 
	int x_high = atoi(argv[3]);
	int y_low = atoi(argv[4]); 
	int y_high = atoi(argv[5]); 

	//Mean variables
	int Rsum = 0;
	int Gsum = 0;
	int Bsum = 0;
	
	int lsum = 0;
	int asum = 0;
	int bsum = 0;

	int count = 0;

	//Range variables
	int highest_R = -1;
	int lowest_R = 1000;
	int highest_G = -1;
	int lowest_G = 1000;
	int highest_B = -1;
	int lowest_B = 1000;

	int highest_l = -1;
	int lowest_l = 1000;
	int highest_a = -1;
	int lowest_a = 1000;
	int highest_b = -1;
	int lowest_b = 1000;

	//Std variables
	double R_diff = 0;
	double G_diff = 0;
	double B_diff = 0;

	double l_diff = 0;
	double a_diff = 0;
	double b_diff = 0;

	//calculate mean/range
	for(int y = y_low; y < y_high; y++){
		for(int x = x_low; x < x_high; x++){
			//get color values
			int R = getRed(&image, x, y);
			int G = getGreen(&image, x, y);
			int B = getBlue(&image, x, y);

			int l = getL(&imglab, x, y);
			int a = getA(&imglab, x, y);
			int b = getB(&imglab, x, y);

			//calc mean
			Rsum += R;
			Gsum += G;
			Bsum += B;

			lsum += l;
			asum += a;
			bsum += b;

			count++;

			//calc range
			if(R > highest_R)
				highest_R = R;
			if(R < lowest_R)
				lowest_R = R;

			if(G > highest_G)
				highest_G = G;
			if(G < lowest_G)
				lowest_G = G;

			if(B > highest_B)
				highest_B = B;
			if(B < lowest_B)
				lowest_B = B;

			if(l > highest_l)
				highest_l = l;
			if(l < lowest_l)
				lowest_l = l;

			if(a > highest_a)
				highest_a = a;
			if(a < lowest_a)
				lowest_a = a;

			if(b > highest_b)
				highest_b = b;
			if(b < lowest_b)
				lowest_b = b;
		}
	}

	//calculate means
	double R_mean = (double) Rsum / count;
	double G_mean = (double) Gsum / count;
	double B_mean = (double) Bsum / count;

	double l_mean = (double) lsum / count;
	double a_mean = (double) asum / count;
	double b_mean = (double) bsum / count;
	
	//calculate std
	for(int y = y_low; y < y_high; y++){
		for(int x = x_low; x < x_high; x++){
			//get color values
			int R = getRed(&image, x, y) - R_mean;
			int G = getGreen(&image, x, y) - G_mean;
			int B = getBlue(&image, x, y) - B_mean;

			int l = getL(&imglab, x, y) - l_mean;
			int a = getA(&imglab, x, y) - a_mean;
			int b = getB(&imglab, x, y) - b_mean;

			//calc diff
			R_diff += R * R;
			G_diff += G * G;
			B_diff += B * B;

			l_diff += l * l;
			a_diff += a * a;
			b_diff += b * b;

		}
	}

	//calculate std
	double R_std = sqrt(R_diff / count);
	double G_std = sqrt(G_diff / count);
	double B_std = sqrt(B_diff / count);
	double l_std = sqrt(l_diff / count);
	double a_std = sqrt(a_diff / count);
	double b_std = sqrt(b_diff / count);

	printf("Pixel count: %d\n", count);
	printf("RGB stats...\n");	
	printf("Average/std Red: %f, %f\n", R_mean, R_std);
	printf("Average/std Green: %f, %f\n", G_mean, G_std);
	printf("Average/std Blue: %f, %f\n", B_mean, B_std);
	printf("Range/Low/High red: %d, %d, %d\n", highest_R - lowest_R, lowest_R, highest_R);
	printf("Range/Low/High green: %d, %d, %d\n", highest_G - lowest_G, lowest_G, highest_G);
	printf("Range/Low/High blue: %d, %d, %d\n\n", highest_B - lowest_B, lowest_B, highest_B);

	printf("lab stats...\n");	
	printf("Average/std l: %f, %f\n", (double)l_mean/255*100, l_std);
	printf("Average/std a: %f, %f\n", a_mean, a_std);
	printf("Average/std b: %f, %f\n", b_mean, b_std);
	printf("Range/Low/High l: %f, %f, %f\n", (double) (highest_l - lowest_l) / 255 * 100, (double)lowest_R/255*100, (double)highest_R/255*100);
	printf("Range/Low/High a: %d, %d, %d\n", highest_a - lowest_a, lowest_a-128, highest_a-128);
	printf("Range/Low/High b: %d, %d, %d\n", highest_b - lowest_b, lowest_b-128, highest_b-128);
	
	return 0;	
}

