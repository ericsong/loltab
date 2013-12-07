#include "extract_data.hpp"

int main(int argc, char **argv) {
	char buffer[100];
	if (argc != 5) {
		cout << "Bad args.\n";
		return 1;
	}
	Mat image = imread(argv[1], 1);
	if (!image.data) {
		cout << "No image here.\n";
		return 1;
	}
	DIR *dp;
	struct dirent *dirItem;

	int *coords = (int *)calloc(2, sizeof(int));
	coords[0] = atoi(argv[2]);
	coords[1] = atoi(argv[3]);
	int rightSideValue = atoi(argv[4]);
	//printf("COORDS: %d %d\n", coords[0], coords[1]);
	int found = coords[0] != -1 && coords[1] != -1;
	if (found) {
		// BUILD IMAGE CACHE
		char directory[] = "../characters/?/";
		char charSet[] = "0123456789%";
		for (int i = 0; i < 11; i++) {
			directory[14] = charSet[i];
			dp = opendir(directory);
			int offset = 0;
			while ((dirItem = readdir(dp)) != NULL) {
				if (dirItem->d_name[0] == '.') {
					continue;
				}
				CHARACTERS[i][offset] = imread(concatStr(directory, dirItem->d_name), 1);
				CHARACTERS_LENGTH[i]++;
				offset++;
			}
			closedir(dp);
		}
		char faded_directory[] = "../faded_characters/?/";
		for (int i = 0; i < 11; i++) {
			faded_directory[20] = charSet[i];
			dp = opendir(faded_directory);
			int offset = 0;
			while ((dirItem = readdir(dp)) != NULL) {
				if (dirItem->d_name[0] == '.') {
					continue;
				}
				FADED_CHARACTERS[i][offset] = imread(concatStr(faded_directory, dirItem->d_name), 1);
				FADED_CHARACTERS_LENGTH[i]++;
				offset++;
			}
			closedir(dp);
		}

		dp = opendir("../items/");
		for (int i = 0; i < ITEMS_LENGTH; i++) {
			dirItem = readdir(dp);
			while (dirItem->d_name[0] == '.') {
				dirItem = readdir(dp);
			}
			ITEMS[i] = imread(concatStr("../items/", dirItem->d_name), 1);
			FADED_ITEMS[i] = imread(concatStr("../faded_items/", dirItem->d_name), 1);
			ITEMS_NAME[i] = copyStr(dirItem->d_name);
		}
		closedir(dp);
		dp = opendir("../summoners/");
		for (int i = 0; i < SUMMONERS_LENGTH; i++) {
			dirItem = readdir(dp);
			while (dirItem->d_name[0] == '.') {
				dirItem = readdir(dp);
			}
			SUMMONERS[i] = imread(concatStr("../summoners/", dirItem->d_name), 1);
			FADED_SUMMONERS[i] = imread(concatStr("../faded_summoners/", dirItem->d_name), 1);
			SUMMONERS_NAME[i] = copyStr(dirItem->d_name);
		}
		closedir(dp);
		
		// after getting the image, get a histogram of the items;
		Mat test_image;// = extremifyImage(image, 127);
		int *avgs;
		int *horizontalHistogram, *verticalHistogram;

		test_image = extremifyImage(image, 72);
		horizontalHistogram = getYHist(test_image);
		avgs = findAverages(horizontalHistogram, image.rows);
		plotHist(horizontalHistogram, image.rows, 768);
		verticalHistogram = getXHist(test_image);
		avgs = findAverages(verticalHistogram, image.cols);
		plotHist(verticalHistogram, image.cols, 768);
		
	}

	return 1;
}
