#ifndef __ImageAlgos_hpp__
#define __ImageAlgos_hpp__

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "ArrayList.hpp"
#include "StringAlgos.hpp"

using namespace cv;

Mat createImage(int width, int height, int channels);
Mat subImage(Mat image, int x1, int y1, int x2, int y2);
Mat resizeImage(Mat image, int width, int height);
Mat recolorImage(Mat image, int brightness, float contrast);
Mat extremifyImage(Mat image, int tolerance);
int averageColor(Mat image);
double compareImage(Mat image1, Mat image2);
void blit(Mat screen, Mat image, int xOffset, int yOffset);
int *getBlob(Mat image, int x, int y, char **visited);
void display(Mat image, char *name);

Mat CHARACTERS[11][20];
Mat FADED_CHARACTERS[11][20];
int CHARACTERS_LENGTH[11];
int FADED_CHARACTERS_LENGTH[11];
#define ITEMS_LENGTH 178
Mat ITEMS[ITEMS_LENGTH];
Mat FADED_ITEMS[ITEMS_LENGTH];
char *ITEMS_NAME[ITEMS_LENGTH];
#define SUMMONERS_LENGTH 13
Mat SUMMONERS[SUMMONERS_LENGTH];
Mat FADED_SUMMONERS[SUMMONERS_LENGTH];
char *SUMMONERS_NAME[SUMMONERS_LENGTH];

#endif
