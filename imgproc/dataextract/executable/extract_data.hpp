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

#define characters_directory		"../../characters/"
#define faded_characters_directory	"../../faded_characters/"
#define items_directory				"../../items/"
#define faded_items_directory		"../../faded_items/"
#define summoners_directory			"../../summoners/"
#define faded_summoners_directory	"../../faded_summoners/"

#define SRX1 0.367
#define SRX2 0.71
#define SRX3 0.78
#define SRX4 0.81
#define SRX5 0.875
#define SRY1 0.218
#define SRY2 0.95

using namespace cv;

#endif
