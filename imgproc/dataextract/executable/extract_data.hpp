#ifndef __extract_data_hpp__
#define __extract_data_hpp__

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "ImageAnalysis.hpp"
#include "HistAlgos.hpp"

#define characters_directory		"../../../imgproc/resources/characters/"
#define faded_characters_directory	"../../../imgproc/resources/faded_characters/"
#define items_directory				"../../../imgproc/resources/items/"
#define faded_items_directory		"../../../imgproc/resources/faded_items/"
#define summoners_directory			"../../../imgproc/resources/summoners/"
#define faded_summoners_directory	"../../../imgproc/resources/faded_summoners/"

#define SRX1 0.367
#define SRX2 0.71
#define SRX3 0.78
#define SRX4 0.81
#define SRX5 0.875
#define SRXL 0.1167
#define SRXR 0.1444
#define SRY1 0.218
#define SRY2 0.95

using namespace cv;

#endif
