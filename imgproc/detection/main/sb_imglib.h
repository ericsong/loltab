#ifndef __SB_IMGLIB_H_INCLUDED__
#define __SB_IMGLIB_H_INCLUDED__

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include "sbcolors.h"

using namespace cv;

int getRed(Mat *image, int x, int y);
int getGreen(Mat *image, int x, int y);
int getBlue(Mat *image, int x, int y);

int getl(Mat *image, int x, int y);
int geta(Mat *image, int x, int y);
int getb(Mat *image, int x, int y);

int getScoreboardLeft(Mat *image, int guess);
int getScoreboardTop(Mat *image, int guess);
int getScoreboardRight(Mat *image, int guess);
int getScoreboardBottom(Mat *image, int guess);

#endif
