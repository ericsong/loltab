#include "extract_data.hpp"

int main(int argc, char **argv) {
	char buffer[100];
	if (argc != 2) {
		fprintf(stderr, "Incorrect usage. Proper usage: ./extract_data <image_name>\n");
		return 1;
	}
	Mat image = imread(argv[1], 1);
	if (!image.data) {
		fprintf(stderr, "No image here.\n");
		return 1;
	}
	DIR *dp;
	struct dirent *dirItem;
	char *charDir = concatStr(characters_directory, "?/");
	int charLength = lenStr(characters_directory);
	char *charDir_faded = concatStr(faded_characters_directory, "?/");
	int charLength_faded = lenStr(faded_characters_directory);

	// BUILD IMAGE CACHE
	char charSet[] = "0123456789%";
	for (int i = 0; i < 11; i++) {
		charDir[charLength] = charSet[i];
		dp = opendir(charDir);
		int offset = 0;
		while ((dirItem = readdir(dp)) != NULL) {
			if (dirItem->d_name[0] == '.') {
				continue;
			}
			CHARACTERS[i][offset] = imread(concatStr(charDir, dirItem->d_name), 1);
			CHARACTERS_LENGTH[i]++;
			offset++;
		}
		closedir(dp);
	}
	for (int i = 0; i < 11; i++) {
		charDir_faded[charLength_faded] = charSet[i];
		dp = opendir(charDir_faded);
		int offset = 0;
		while ((dirItem = readdir(dp)) != NULL) {
			if (dirItem->d_name[0] == '.') {
				continue;
			}
			FADED_CHARACTERS[i][offset] = imread(concatStr(charDir_faded, dirItem->d_name), 1);
			FADED_CHARACTERS_LENGTH[i]++;
			offset++;
		}
		closedir(dp);
	}

	dp = opendir(items_directory);
	for (int i = 0; i < ITEMS_LENGTH; i++) {
		dirItem = readdir(dp);
		while (dirItem->d_name[0] == '.') {
			dirItem = readdir(dp);
		}
		ITEMS[i] = imread(concatStr(items_directory, dirItem->d_name), 1);
		FADED_ITEMS[i] = imread(concatStr(faded_items_directory, dirItem->d_name), 1);
		ITEMS_NAME[i] = copyStr(dirItem->d_name);
	}
	closedir(dp);
	dp = opendir(summoners_directory);
	for (int i = 0; i < SUMMONERS_LENGTH; i++) {
		dirItem = readdir(dp);
		while (dirItem->d_name[0] == '.') {
			dirItem = readdir(dp);
		}
		SUMMONERS[i] = imread(concatStr(summoners_directory, dirItem->d_name), 1);
		FADED_SUMMONERS[i] = imread(concatStr(faded_summoners_directory, dirItem->d_name), 1);
		SUMMONERS_NAME[i] = copyStr(dirItem->d_name);
	}
	closedir(dp);

	// after getting the image, get a histogram of the items;
	Mat test_image = extremifyImage(image, 72);
	int *avgs;
	int *horizontalHistogram, *verticalHistogram;

	horizontalHistogram = getYHist(test_image);
	avgs = findAverages(horizontalHistogram, image.rows);
	verticalHistogram = getXHist(test_image);
	avgs = findAverages(verticalHistogram, image.cols);

	return 0;
}
