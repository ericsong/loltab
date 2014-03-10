#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

static int border_tolerance; /* inputted from the user */

int isSimilar(Vec3b p1, Vec3b p2) {
	int dr = p2[2] - p1[2];
	dr *= dr;
	int db = p2[0] - p1[0];
	db *= db;
	int dg = p2[1] - p1[1];
	dg *= dg;
	return (dr + db + dg) < border_tolerance;
}

int *cornerValues(Mat image) {
	int rows = image.rows;
	int y = rows / 2;
	int x = 0;
	int topMost, bottomMost, rightMost, leftMost;
	// traverse to top
	for (int sy = y; y > 1; y--) {
		if (isSimilar(
			image.at<Vec3b>(sy, x),
			image.at<Vec3b>(sy - 1, x)
			) {
				continue;
		} else {
			topMost = sy;
			break;
		}
	}
}
