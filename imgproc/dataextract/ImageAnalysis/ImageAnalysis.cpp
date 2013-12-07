#include "ImageAnalysis.hpp"

char *readSubExpression(Mat image, int faded) {
	char *expression = (char *)calloc(4, sizeof(char));
	expression[1] = '\0';
	double error = 1.0;
	char expr[] = "?";
	char charSet[] = "0123456789/";
	for (int i = 0; i < 11; i++) {
		expr[0] = charSet[i];
		double total_error = matchString(image, expr, faded);
		if (total_error < error) {
			expression[0] = charSet[i];
			error = total_error;
		}
	}
	if (image.cols > 4) {
		char dexpr[] = "??";
		char dcharSet[] = "/023456789";
		for (int x = 0; x < 10; x++) {
			for (int y = 0; y < 10; y++) {
				if (x == y) {
					continue;
				}
				dexpr[0] = dcharSet[x];
				dexpr[1] = dcharSet[y];
				double total_error = matchString(image, dexpr, faded);
				if (total_error < error) {
					expression[0] = dcharSet[x];
					expression[1] = dcharSet[y];
					expression[2] = '\0';
					error = total_error;
				}
			}
		}
	}
	return expression;
}

char *readExpression(Mat image, int x1, int y1, int x2, int y2, char faded, int *indexER) {
	Mat new_image;
	int brightness = 0;
	float contrast = 1.5;
	new_image = subImage(image, x1, y1, x2, y2);
	new_image = recolorImage(new_image, brightness, contrast);
	if (!faded) {
		new_image = extremifyImage(new_image, 128);
	} else {
		new_image = extremifyImage(new_image, 64);
	}
		
	char fname[] = "00000000.bmp";
	int index = *indexER;
	char **visited = (char **)calloc(new_image.rows, sizeof(char *));
	for (int i = 0; i < new_image.rows; i++) {
		visited[i] = (char *)calloc(new_image.cols, sizeof(char));
	}
	char *buffer = (char *)calloc(25, sizeof(char));
	buffer[24] = '\0';
	int offset = 0;
	for (int x = 0; x < new_image.cols; x++) {
		for (int y = 0; y < new_image.rows; y++) {
			if (!visited[y][x] && (
				new_image.at<Vec3b>(y, x)[0] == 255 ||
				new_image.at<Vec3b>(y, x)[1] == 255 ||
				new_image.at<Vec3b>(y, x)[2] == 255)) {
					int *coords = getBlob(new_image, x, y, visited);
					Mat pnum = subImage(image, x1 + coords[0], y1 + coords[1], x1 + coords[2], y1 + coords[3]);
					if (pnum.rows < 4 || pnum.cols > 20) {
						continue;
					}
					char *n = readSubExpression(pnum, faded);
					for (int i = 0; n[i] != '\0'; i++) {
						buffer[offset] = n[i];
						offset++;
					}
					index++;
					int temp = index;
					for (int i = 0; i < 8; i++) {
						fname[7 - i] = temp % 10 + '0';
						temp /= 10;
					}
			}
			visited[y][x] = 1;
		}
	}
	buffer[offset] = '\0';
	*indexER = index;
	return buffer;
}

char *identifyItem(Mat image) {
	return searchCache(ITEMS, ITEMS_LENGTH, ITEMS_NAME, image);
}

char *identifyFadedItem(Mat image) {
	return searchCache(FADED_ITEMS, ITEMS_LENGTH, ITEMS_NAME, image);
}

char *identifySummoner(Mat image) {
	return searchCache(SUMMONERS, SUMMONERS_LENGTH, SUMMONERS_NAME, image);
}

char *identifyFadedSummoner(Mat image) {
	return searchCache(FADED_SUMMONERS, SUMMONERS_LENGTH, SUMMONERS_NAME, image);
}

char *searchCache(Mat cache[], int cacheLength, char *names[], Mat image) {
	char *name = "None";
	float total_error = 1.0;
	Mat new_image = image;
	for (int i = 0; i < cacheLength; i++) {
		if (new_image.rows != cache[i].rows || new_image.cols != cache[i].cols) {
			new_image = resizeImage(new_image, cache[i].cols, cache[i].rows);
		}
		float error = compareImage(cache[i], image);
		if (error < total_error) {
			total_error = error;
			name = names[i];
		}
	}
	return name;
}

double matchString(Mat image, char str[], int faded) {
	double error = 1.0;
	int i, length = 0;
	for (i = 0; str[i] != '\0'; i++) {
		if (!isValidForMatch(str[i])) {
			return 1.0;
		}
		length++;
	}
	if (length == 0) {
		return 1.0;
	}
	int index[length];
	for (i = 0; i < length; i++) {
		if (str[i] != '/') {
			index[i] = str[i] - '0';
		} else {
			index[i] = 10;
		}
	}

	// compute least error based on all the combinations
	int total_combinations = 1;
	for (i = 0; i < length; i++) {
		if (!faded) {
			total_combinations *= CHARACTERS_LENGTH[index[i]];
		} else {
			total_combinations *= FADED_CHARACTERS_LENGTH[index[i]];
		}
	}
	for (i = 0; i < total_combinations; i++) {
		double total_error = 1.0;
		int combination = i, total_width = 0, digit;
		for (digit = 0; digit < length; digit++) {
			int n = index[digit];
			if (!faded) {
				total_width += CHARACTERS[n][combination % CHARACTERS_LENGTH[n]].cols;
				combination /= CHARACTERS_LENGTH[n];
			} else {
				total_width += FADED_CHARACTERS[n][combination % FADED_CHARACTERS_LENGTH[n]].cols;
				combination /= FADED_CHARACTERS_LENGTH[n];
			}
		}
		combination = i;
		Mat word = createImage(total_width, image.rows, image.channels());
		int offset = 0;
		for (digit = 0; digit < length; digit++) {
			int n = index[digit];
			if (!faded) {
				Mat pnum = CHARACTERS[n][combination % CHARACTERS_LENGTH[n]];
				blit(word, resizeImage(pnum, pnum.cols, image.rows), offset, 0);
				combination /= CHARACTERS_LENGTH[n];
				offset += pnum.cols;
			} else {
				Mat pnum = FADED_CHARACTERS[n][combination % FADED_CHARACTERS_LENGTH[n]];
				blit(word, resizeImage(pnum, pnum.cols, image.rows), offset, 0);
				combination /= FADED_CHARACTERS_LENGTH[n];
				offset += pnum.cols;
			}
		}
		word = resizeImage(word, image.cols, word.rows);
		total_error = compareImage(word, image);
		if (total_error < error) {
			error = total_error;
		}
	}
	return error;
}

char isValidForMatch(char c) {
	return (c == '/') || ('0' <= c && c <= '9');
}
