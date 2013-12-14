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

	int subImageOffset_y = round(multiply(SRY1, image.rows));
	Mat itemSubImage = subImage(image, round(multiply(SRX1, image.cols)),
								subImageOffset_y,
								round(multiply(SRX2, image.cols)),
								round(multiply(SRY2, image.rows)));
	Mat test_image = extremifyImage(itemSubImage, 72);
	int *dimensions = getDimensions(test_image);
	int size = dimensions[19];
	if (size == 0) {
		fprintf(stderr, "Got a size of 0\n");
		return 1;
	}
	char **summoners = (char **)calloc(20, sizeof(char *));
	char **items = (char **)calloc(70, sizeof(char *));
	for (int i = 0; i < 10; i++) {
		if (dimensions[20 + i]) {
			summoners[i * 2] = identifySummoner(subImage(itemSubImage,
										dimensions[10],
										dimensions[i],
										dimensions[10] + size,
										dimensions[i] + size));
			summoners[i * 2 + 1] = identifySummoner(subImage(itemSubImage,
										dimensions[11],
										dimensions[i],
										dimensions[11] + size,
										dimensions[i] + size));
			for (int n = 0; n < 7; n++) {
				items[i * 7 + n] = identifyItem(subImage(itemSubImage,
										dimensions[12 + n],
										dimensions[i],
										dimensions[12 + n] + size,
										dimensions[i] + size));
			}
		} else {
			summoners[i * 2] = identifyFadedSummoner(subImage(itemSubImage,
										dimensions[10],
										dimensions[i],
										dimensions[10] + size,
										dimensions[i] + size));
			summoners[i * 2 + 1] = identifyFadedSummoner(subImage(itemSubImage,
										dimensions[11],
										dimensions[i],
										dimensions[11] + size,
										dimensions[i] + size));
			for (int n = 0; n < 7; n++) {
				items[i * 7 + n] = identifyFadedItem(subImage(itemSubImage,
										dimensions[12 + n],
										dimensions[i],
										dimensions[12 + n] + size,
										dimensions[i] + size));
			}
		}
	}

	char **scores = (char **)calloc(10, sizeof(char *));
	char **creeps = (char **)calloc(10, sizeof(char *));
	for (int i = 0; i < 10; i++) {
		scores[i] = getExpression(subImage(image, round(multiply(SRX2, image.cols)),
		//scores[i] = readExpression(image, round(multiply(SRX2, image.cols)),
											subImageOffset_y + dimensions[i],
											round(multiply(SRX3, image.cols)),
											subImageOffset_y + dimensions[i] + size),
										!dimensions[20 + i]);
		creeps[i] = getExpression(subImage(image, round(multiply(SRX4, image.cols)),
		//creeps[i] = readExpression(image, round(multiply(SRX4, image.cols)),
											subImageOffset_y + dimensions[i],
											round(multiply(SRX5, image.cols)),
											subImageOffset_y + dimensions[i] + size),
										!dimensions[20 + i]);
	}

	return 0;
}
