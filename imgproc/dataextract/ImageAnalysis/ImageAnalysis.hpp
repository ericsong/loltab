#ifndef __ImageAnalysis_hpp__
#define __ImageAnalysis_hpp__

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "ImageAlgos.hpp"

using namespace cv;

char *readSubExpression(Mat image, int faded);
char *readExpression(Mat image, int x1, int y1, int x2, int y2, char faded, int *index);
char *searchCache(Mat *cache, int length, char *names[], Mat image);
char *identifyItem(Mat image);
char *identifyFadedItem(Mat image);
char *identifySummoner(Mat image);
char *identifyFadedSummoner(Mat image);
char isValidForMatch(char c);
double matchString(Mat, char *, int);

#endif
