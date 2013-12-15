#ifndef __HistAlgos_hpp__
#define __HistAlgos_hpp__

#include <stdlib.h>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "ImageAlgos.hpp"

using namespace cv;

int *getXHist(Mat image);
int *getYHist(Mat image);
void plotHist(int *array, int length, char *name);
void printHist(int *array, int length);
int getMax(int *array, int length);
int *subArray(int *array, int beg, int end);
int *findLength(int *histogram, int length);
int *distribute (int *dataset, int size, int length);
int *getDimensions(Mat image);
char *getExpression(Mat image, int faded);
char matchImage(Mat image, int faded);
int getMean(int *array, int length);
int getSum(int *array, int length);
float multiply(float x, float y);
float divide(float x, float y);
int round(float x);
int ceiling(float x);

#endif
