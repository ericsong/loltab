#ifndef __HistAlgos_hpp__
#define __HistAlgos_hpp__

#include <stdlib.h>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "ImageAlgos.hpp"

using namespace cv;

int getMax(int *array, int length);
int *getXHist(Mat image);
int *getYHist(Mat image);
void plotHist(int *array, int length, int scale);
int *findAverages(int *histogram, int length);
int mean(int *array);
int MUL(float x, float y);
int DIV(float x, float y);

#endif
