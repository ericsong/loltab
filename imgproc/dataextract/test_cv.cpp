#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>

using namespace cv;

int main() 
{
	Mat image = imread("/home/reggi/tmp/dyrus/img21.jpg", 1);

	printf("%d\n", image.at<Vec3b>(50, 31)[0] );
	printf("%d\n", image.at<Vec3b>(50, 31)[1] );
	printf("%d\n", image.at<Vec3b>(50, 31)[2] );


	return 0;	
}
