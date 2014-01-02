#include "extract_data.hpp"

int main(int argc, char **argv) {
	char buffer[100];
	if (argc != 3) {
		fprintf(stderr, "Incorrect usage. Proper usage: ./extract_data <image_name> <name of output file>. To output to stdout, declare name of output file to be \"stdout\"\n");
		return 1;
	}
	Mat image = imread(argv[1], 1);
	FILE *outputFile;
	char *outputFilename;
	if (!image.data) {
		fprintf(stderr, "No image here.\n");
		return 1;
	}
	outputFilename = argv[2];
	if (strEquals(outputFilename, "stdout")) {
		outputFile = stdout;
	} else {
		outputFile = fopen(outputFilename, "w");
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
	//display(itemSubImage, "subby");
	Mat test_image = extremifyImage(itemSubImage, 72);
	int *dimensions = getDimensions(test_image);
	/*Mat display_image = createImage(test_image.cols, test_image.rows, 3);
	for (int y = 0; y < 10; y++) {
		for (int x = 10; x < 18; x++) {
			for (int r = dimensions[y]; r < dimensions[y] + dimensions[19]; r++) {
				for (int c = dimensions[x]; c < dimensions[x] + dimensions[19]; c++) {
					display_image.at<Vec3b>(r, c)[0] = 255;
					display_image.at<Vec3b>(r, c)[1] = 255;
				}
			}
		}
	}
	display(display_image, "asdfaf");
	printHist(dimensions, 20);*/
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
	int *kills = (int *)calloc(10, sizeof(int));
	int *deaths = (int *)calloc(10, sizeof(int));
	int *assists = (int *)calloc(10, sizeof(int));
	int *creeps = (int *)calloc(10, sizeof(int));
	int *levels = (int *)calloc(10, sizeof(int));
	for (int i = 0; i < 10; i++) {
		scores[i] = getExpression(subImage(image, round(multiply(SRX2, image.cols)),
		//scores[i] = readExpression(image, round(multiply(SRX2, image.cols)),
											subImageOffset_y + dimensions[i],
											round(multiply(SRX3, image.cols)),
											subImageOffset_y + dimensions[i] + size),
											!dimensions[20 + i]);
		int slashIndex1 = indexOf(scores[i], '/');
		int slashIndex2 = indexOf(subStr(	scores[i],
											slashIndex1 + 1,
											lenStr(scores[i])),
								  '/') + slashIndex1 + 1;
		if (slashIndex1 != -1 && slashIndex2 != -1) {
			kills[i] = atoi(subStr(scores[i], 0, slashIndex1));
			if (kills[i] > 100) {
				kills[i] = -1; //error
			}
			deaths[i] = atoi(subStr(scores[i], slashIndex1 + 1, slashIndex2));
			if (deaths[i] > 100) {
				deaths[i] = -1; //error
			}
			assists[i] = atoi(subStr(scores[i], slashIndex2 + 1, lenStr(scores[i])));
			if (assists[i] > 100) {
				assists[i] = -1; //error 
			}
		}
		creeps[i] = atoi(getExpression(subImage(image, round(multiply(SRX4, image.cols)),
		//creeps[i] = readExpression(image, round(multiply(SRX4, image.cols)),
											subImageOffset_y + dimensions[i],
											round(multiply(SRX5, image.cols)),
											subImageOffset_y + dimensions[i] + size),
											!dimensions[20 + i]));
		levels[i] = atoi(getExpression(subImage(image, round(multiply(SRXL, image.cols)),
											subImageOffset_y + dimensions[i],
											round(multiply(SRXR, image.cols)),
											subImageOffset_y + dimensions[i] + size),
											!dimensions[20 + i]));
		if (levels[i] < 1 || levels[i] > 18) {
			levels[i] = 0;
		}
	}
	char **players = (char **)calloc(10, sizeof(char *));

	// output the data to a text file
	fprintf(outputFile, "!!!START!!!\n{\n");
	fprintf(outputFile, "\t\"players\": [\n");
	for (int i = 0; i < 10; i++) {
		fprintf(outputFile, "\t\t{\n");
		fprintf(outputFile, "\t\t\t\"name\": \"%s\",\n", append("player", i + '0'));
		fprintf(outputFile, "\t\t\t\"champion\": \"%s\",\n", append("champion", i + '0'));
		fprintf(outputFile, "\t\t\t\"level\": \"%d\",\n", levels[i]);
		fprintf(outputFile, "\t\t\t\"spell1\": \"%s\",\n", summoners[i * 2]);
		fprintf(outputFile, "\t\t\t\"spell2\": \"%s\",\n", summoners[i * 2 + 1]);
		fprintf(outputFile, "\t\t\t\"kills\": \"%d\",\n", kills[i]);
		fprintf(outputFile, "\t\t\t\"deaths\": \"%d\",\n", deaths[i]);
		fprintf(outputFile, "\t\t\t\"assists\": \"%d\",\n", assists[i]);
		fprintf(outputFile, "\t\t\t\"cs\": \"%d\",\n", creeps[i]);
		fprintf(outputFile, "\t\t\t\"items\": [\n");
		for (int n = 0; n < 7; n++) {
			fprintf(outputFile, "\t\t\t\t\"%s\"", items[n + i * 7]);
			if (n != 6) {
				fprintf(outputFile, ",");
			}
			fprintf(outputFile, "\n");
		}
		fprintf(outputFile, "\t\t\t]\n\t\t}");
		if (i != 9) {
			fprintf(outputFile, ",");
		}
		fprintf(outputFile, "\n");
	}
	fprintf(outputFile, "\t]\n}\n!!!END!!!\n");

	return 0;
}
