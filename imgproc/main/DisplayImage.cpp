#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;

typedef struct ArrayList ArrayList;
struct ArrayList {
	long *data;
	unsigned long size;
	unsigned long length;
};

ArrayList *CreateList();
void ALInsert(ArrayList *, long);
long ALGet(ArrayList *, unsigned long);
void DestroyList(ArrayList *);

char *concatStr(char *, char *);
int lenStr(char *);
char *copyStr(char *);
char *subStr(char *, int, int);

Mat createImage(int, int, int);
Mat subImage(Mat, int, int, int, int);
Mat resizeImage(Mat, int, int);
Mat recolorImage(Mat, int, float);
Mat extremifyImage(Mat, int);
int averageColor(Mat);

double compareImage(Mat, Mat);
void blit(Mat, Mat, int, int);
double matchString(Mat, char *, int);
int *getBlob(Mat, int, int, char **);
char *readSubExpression(Mat, int);
char *readExpression(Mat, int, int, int, int, char *, int *);

int *findLocation(Mat, Mat);
char *identifyChampion(Mat);
char *identifyItem(Mat);
char *identifySummoner(Mat);

Mat CHARACTERS[11][20];
Mat FADED_CHARACTERS[11][20];
int CHARACTERS_LENGTH[11];
int FADED_CHARACTERS_LENGTH[11];
#define CHAMPIONS_LENGTH 116
Mat CHAMPIONS[CHAMPIONS_LENGTH];
char *CHAMPIONS_NAME[CHAMPIONS_LENGTH];
#define ITEMS_LENGTH 173
Mat ITEMS[ITEMS_LENGTH];
char *ITEMS_NAME[ITEMS_LENGTH];
#define SUMMONERS_LENGTH 13
Mat SUMMONERS[SUMMONERS_LENGTH];
char *SUMMONERS_NAME[SUMMONERS_LENGTH];

void display(Mat image) {
	namedWindow("asdf", CV_WINDOW_AUTOSIZE);
	imshow("asdf", image);
	waitKey(0);
}

int main(int argc, char **argv) {
	// NOTE: YOU WILL HAVE TO CACHE IMAGES LATER ON TO MAKE THIS FASTER
	char buffer[100];
	if (argc != 2) {
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

	// BUILD IMAGE CACHE
	cout << "loading cache" << endl;
	char directory[] = "../1080p/characters/?/";
	char charSet[] = "0123456789%";
	for (int i = 0; i < 11; i++) {
		directory[20] = charSet[i];
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
	char faded_directory[] = "../1080p/faded_characters/?/";
	for (int i = 0; i < 11; i++) {
		faded_directory[26] = charSet[i];
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
	dp = opendir("../champions/");
	for (int i = 0; i < CHAMPIONS_LENGTH; i++) {
		dirItem = readdir(dp);
		while (dirItem->d_name[0] == '.') {
			dirItem = readdir(dp);
		}
		CHAMPIONS[i] = resizeImage(imread(concatStr("../champions/", dirItem->d_name), 1), 28, 28);
		CHAMPIONS_NAME[i] = copyStr(dirItem->d_name);//, 0, lenStr(dirItem->d_name - 4));
	}
	closedir(dp);
	dp = opendir("../items/");
	for (int i = 0; i < ITEMS_LENGTH; i++) {
		dirItem = readdir(dp);
		while (dirItem->d_name[0] == '.') {
			dirItem = readdir(dp);
		}
		ITEMS[i] = resizeImage(imread(concatStr("../items/", dirItem->d_name), 1), 28, 28);
		ITEMS_NAME[i] = copyStr(dirItem->d_name);//, 0, lenStr(dirItem->d_name - 4));
	}
	closedir(dp);
	dp = opendir("../summoners/");
	for (int i = 0; i < SUMMONERS_LENGTH; i++) {
		dirItem = readdir(dp);
		while (dirItem->d_name[0] == '.') {
			dirItem = readdir(dp);
		}
		SUMMONERS[i] = resizeImage(imread(concatStr("../summoners/", dirItem->d_name), 1), 28, 28);
		SUMMONERS_NAME[i] = copyStr(dirItem->d_name);//, 0, lenStr(dirItem->d_name - 4));
	}
	closedir(dp);

	cout << "cache loaded" << endl;

	// ACQUIRE DATA
	//int x1 = 650, x2 = 700, y1 = 175, y2 = 200;
	//int x1 = 1180, y1 = 350, x2 = 1337, y2 = 384;
	//cout << compareImage(resizeImage(imread("../champions/Karma.png", 1), 28, 28), subImage(image, 604, 360, 604 + 28, 360 + 28)) << endl;
	/*cout << "upper left hand corner x value: ";
	cin.getline(buffer, 100);
	x1 = atoi(buffer);
	cout << "upper left hand corner y value: ";
	cin.getline(buffer, 100);
	y1 = atoi(buffer);
	cout << "bot right hand corner x value: ";
	cin.getline(buffer, 100);
	x2 = atoi(buffer);
	cout << "bot right hand corner y value: ";
	cin.getline(buffer, 100);
	y2 = atoi(buffer);

	char faded = 0;
	int index = 0;
	char *expression = readExpression(image, x1, y1, x2, y2, &faded, &index);
	cout << expression << endl;*/

	Mat new_image = imread("../IMG.bmp", 1);
	//display(new_image);
	//display(image);
	int *coords = findLocation(image, imread("../IMG.bmp", 1));
	int found = coords[0] != -1 && coords[1] != -1;
	if (found) {
		int cx1_offset = -7, cy1_offset = 50;
		int height = 28;
		int x1 = coords[0] + cx1_offset, y1 = coords[1] + cy1_offset;
		int offset1 = 49, offset2 = 272;
		int g1 = 30, g2 = 570, g3 = 670;
		int l1 = 65, l2 = 630, l3 = 720;
		int s1 = 289, s2 = 321;
		int itemLocations[7] = {356, 386, 416, 446, 476, 506, 536};

		int index = 0;
		for (int i = 0; i < 10; i++) {
			cout << " ================================================== \n";
			int top = y1 + (offset1 * i), bot = top + height;
			if (i >= 5) {
				int delta = offset2 - (5 * offset1);
				top += delta;
				bot += delta;
			}
			char faded = 0;
			char *champion = identifyChampion(subImage(image, x1, top, x1 + height, bot));
			char *summoner1 = identifySummoner(subImage(image, x1 + s1, top, x1 + s1 + height, bot));
			char *summoner2 = identifySummoner(subImage(image, x1 + s2, top, x1 + s2 + height, bot));
			char *items[7];
			for (int n = 0; n < 7; n++) {
				items[n] = identifyItem(subImage(image, x1 + itemLocations[n], top, x1 + itemLocations[n] + height, bot));
			}
			char *level = readExpression(image, x1 + g1, top, x1 + l1, bot, &faded, &index);
			char *KDR = readExpression(image, x1 + g2, top, x1 + l2, bot, &faded, &index);
			char *cs = readExpression(image, x1 + g3, top, x1 + l3, bot, &faded, &index);
			printf("champion: %s, summoners: %s %s, level: %s, KDR: %s, cs: %s, items:\n", champion, summoner1, summoner2, level, KDR, cs);
			for (int n = 0; n < 7; n++) {
				printf("%s\n", items[n]);
			}
		}
	}


	return 1;
}

int averageColor(Mat image) {
	unsigned int color = 0;
	for (int y = 0; y < image.rows; y++) {
		for (int x = 0; x < image.cols; x++) {
			for (int c = 0; c < image.channels(); c++) {
				color += image.at<Vec3b>(y, x)[c];
			}
		}
	}
	return color / (image.rows * image.cols);
}


/** ArrayList **/

ArrayList *CreateList() {
	ArrayList *list = (ArrayList *)calloc(1, sizeof(ArrayList));
	return list;
}

void ALInsert(ArrayList *list, long data) {
	if (list == NULL) {
		return;
	}
	if (list->length == list->size) {
		if (list->size == 0) {
			list->size = 1;
			list->data = (long *)calloc(1, sizeof(long));
		} else {
			list->size *= 2;
			list->data = (long *)realloc(list->data, list->size * sizeof(long));
		}
	}
	list->data[list->length++] = data;
}

void ALPlaceIn(ArrayList *list) {
	
}

long ALGet(ArrayList *list, unsigned long index) {
	if (list == NULL || index >= list->length) {
		return 0;
	}
	return list->data[index];
}

void DestroyList(ArrayList *list) {
	if (list == NULL) {
		return;
	}
	if (list->data != NULL) {	
		free(list->data);
	}
	free(list);
}

char *concatStr(char *c1, char *c2) {
	char *c3;
	int length1 = 0, length2 = 0, length = 0, i;
	for (i = 0; c1[i] != '\0'; i++) {
		length1++;
	}
	for (i = 0; c2[i] != '\0'; i++) {
		length2++;
	}
	length = length1 + length2 + 1;
	c3 = (char *)calloc(length, sizeof(char));
	for (i = 0; i < length1; i++) {
		c3[i] = c1[i];
	}
	for (i = 0; i < length2; i++) {
		c3[i + length1] = c2[i];
	}
	c3[length - 1] = '\0';
	return c3;
}

char *subStr(char *s, int beg, int end) {
	char *new_s = (char *)calloc(end - beg + 1, sizeof(char));
	for (int i = beg; i < end; i++) {
		new_s[i - beg] = s[i];
	}
	new_s[end - beg] = '\0';
	return new_s;
}

Mat createImage(int width, int height, int channels) {
	Mat new_image;
	new_image.create(height, width, CV_8UC(channels));
	return new_image;
}

Mat subImage(Mat image, int x1, int y1, int x2, int y2) {
	int dx = x2 - x1;
	int dy = y2 - y1;
	if (dx <= 0 || dy <= 0 || y2 > image.rows || x2 > image.cols) {
		return image;
	}
	Mat new_image = createImage(dx, dy, image.channels());
	for (int y = y1; y < y2; y++) {
		for (int x = x1; x < x2; x++) {
			for (int c = 0; c < image.channels(); c++) {
				new_image.at<Vec3b>(y - y1, x - x1)[c] =
					image.at<Vec3b>(y, x)[c];
			}
		}
	}
	return new_image;
}

Mat resizeImage(Mat image, int width, int height) {
	Mat new_image = createImage(width, height, image.channels());
	float xscale = (((float)width) / ((float)image.cols));
	float yscale = (((float)height) / ((float)image.rows));
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int c = 0; c < image.channels(); c++) {
				new_image.at<Vec3b>(y, x)[c] = image.at<Vec3b>(
					(int)(((float)y) / yscale),
					(int)(((float)x) / xscale))[c];
			}
		}
	}
	return new_image;
}

Mat recolorImage(Mat image, int brightness, float contrast) {
	int changed[256];
	for (int i = 0; i < 256; i++) {
		changed[i] = saturate_cast<unsigned char>(contrast * i + brightness);
		if (changed[i] <= 0) {
			changed[i] = 0;
		} else if (changed[i] > 255) {
			changed[i] = 255;
		}
	}
	unsigned char lookup[256];
	for (int i = 0; i < 256; i++) {
		lookup[i] = changed[i];
	}
	Mat new_image = createImage(image.cols, image.rows, image.channels());
	for (int y = 0; y < image.rows; y++) {
		for (int x = 0; x < image.cols; x++) {
			for (int c = 0; c < image.channels(); c++) {
				new_image.at<Vec3b>(y, x)[c] = lookup[image.at<Vec3b>(y, x)[c]];
			}
		}
	}
	return new_image;
}

double compareImage(Mat image1, Mat image2) {
	double error = 0.00, max_error = image1.rows * image1.cols * 255.0 * 255.0 * image1.channels();
	for (int y = 0; y < image1.rows; y++) {
		for (int x = 0; x < image1.cols; x++) {
			for (int c = 0; c < image1.channels(); c++) {
				int difference = image1.at<Vec3b>(y, x)[c] - image2.at<Vec3b>(y, x)[c];
				error += (double)(difference * difference);
			}
		}
	}
	return error / max_error;
}

Mat extremifyImage(Mat image, int tolerance) {
	unsigned char lookup[256];
	for (int i = 0; i < tolerance; i++) {
		lookup[i] = 0;
	}
	for (int i = tolerance; i < 256; i++) {
		lookup[i] = 255;
	}
	Mat new_image = createImage(image.cols, image.rows, image.channels());
	for (int y = 0; y < image.rows; y++) {
		for (int x = 0; x < image.cols; x++) {
			for (int c = 0; c < image.channels(); c++) {
				new_image.at<Vec3b>(y, x)[c] = lookup[image.at<Vec3b>(y, x)[c]];
			}
		}
	}
	return new_image;
}

int *getBlob(Mat image, int x, int y, char **visited) {
	ArrayList *X = CreateList();
	ArrayList *Y = CreateList();
	ALInsert(X, x);
	ALInsert(Y, y);
	int length = 0, lx = x, ly = y, hx = x, hy = y;
	visited[y][x] = 1;
	while (length < X->length) {
		// add to paired list
		x = ALGet(X, length);
		y = ALGet(Y, length);
		if (image.at<Vec3b>(y, x)[0] == 255 ||
			image.at<Vec3b>(y, x)[1] == 255 ||
			image.at<Vec3b>(y, x)[2] == 255) {
			if (((x + 1) < image.cols) && !(visited[y][x + 1])) {
				ALInsert(X, x + 1);
				ALInsert(Y, y);
				visited[y][x + 1] = 1;
			}
			if (((y + 1) < image.rows) && !(visited[y + 1][x])) {
				ALInsert(X, x);
				ALInsert(Y, y + 1);
				visited[y + 1][x] = 1;
			}
			if (((x - 1) >= 0) && !(visited[y][x - 1])) {
				ALInsert(X, x - 1);
				ALInsert(Y, y);
				visited[y][x - 1] = 1;
			}
			if (((y - 1) >= 0) && !(visited[y - 1][x])) {
				ALInsert(X, x);
				ALInsert(Y, y - 1);
				visited[y - 1][x] = 1;
			}
			if (((x + 1) < image.cols) && ((y - 1) >= 0) && !(visited[y - 1][x + 1])) {
				ALInsert(X, x + 1);
				ALInsert(Y, y - 1);
				visited[y - 1][x + 1] = 1;
			}
			if (((x - 1) >= 0) && ((y + 1) < image.rows) && !(visited[y + 1][x - 1])) {
				ALInsert(X, x - 1);
				ALInsert(Y, y + 1);
				visited[y + 1][x - 1] = 1;
			}
			if (x < lx) {
				lx = x;
			} else if (x > hx) {
				hx = x;
			}
			if (y < ly) {
				ly = y;
			} else if (y > hy) {
				hy = y;
			}
		}
		length++;
	}
	int *coords = (int *)calloc(4, sizeof(int));
	coords[0] = lx;
	coords[1] = ly;
	coords[2] = hx + 1;
	coords[3] = hy + 1;
	DestroyList(X);
	DestroyList(Y);
	return coords;
}




void blit(Mat screen, Mat image, int x_offset, int y_offset) {
	int rows = image.rows;
	if (rows + y_offset > screen.rows) {
		rows = screen.rows - y_offset;
	}
	int cols = image.cols;
	if (cols + x_offset > screen.cols) {
		cols = screen.cols - x_offset;
	}
	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {
			for (int c = 0; c < image.channels(); c++) {
				screen.at<Vec3b>(y + y_offset, x + x_offset)[c] = image.at<Vec3b>(y, x)[c];
			}
		}
	}
}

char isValidForMatch(char c) {
	return (c == '/') || ('0' <= c && c <= '9');
}

double matchString(Mat image, char str[], int faded) {
	// oh boy (use getNumber as inspiration)
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
				blit(word, CHARACTERS[n][combination % CHARACTERS_LENGTH[n]], offset, 0);
				combination /= CHARACTERS_LENGTH[n];
				offset += CHARACTERS[n][combination % CHARACTERS_LENGTH[n]].cols;
			} else {
				blit(word, FADED_CHARACTERS[n][combination % FADED_CHARACTERS_LENGTH[n]], offset, 0);
				combination /= FADED_CHARACTERS_LENGTH[n];
				offset += FADED_CHARACTERS[n][combination % FADED_CHARACTERS_LENGTH[n]].cols;
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

int lenStr(char *str) {
	int length = 0;
	while (str[length] != '\0') {
		length++;
	}
	return length;
}

char *copyStr(char *str) {
	int length = lenStr(str);
	char *new_str = (char *)calloc(length + 1, sizeof(char));
	for (int i = 0; i < length; i++) {
		new_str[i] = str[i];
	}
	new_str[length] = '\0';
	return new_str;
}

char *readSubExpression(Mat image, int faded) {
	char *expression = (char *)calloc(4, sizeof(char));
	expression[1] = '\0';
	// try to do only numbers first
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

char *readExpression(Mat image, int x1, int y1, int x2, int y2, char *faded, int *indexER) {
	Mat new_image;
	int brightness = 0;
	float contrast = 1.5;
	new_image = subImage(image, x1, y1, x2, y2);
	new_image = recolorImage(new_image, brightness, contrast);
	int avg = 0;
	if ((avg = averageColor(extremifyImage(new_image, 128))) < 33) {
		*faded = 1;
	}
	printf("Average: %d\n", avg);
	if (!(*faded)) {
		cout << "128\n";
		new_image = extremifyImage(new_image, 128);
	} else {
		cout << "64\n";
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
				if (pnum.rows < 4) {
					continue;
				}
				char *n = readSubExpression(pnum, *faded);
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
				printf("%s: %s\n", fname, n);
				imwrite(fname, pnum);
			}
			visited[y][x] = 1;
		}
	}
	buffer[offset] = '\0';
	*indexER = index;
	return buffer;
}

int *findLocation(Mat image, Mat base) {
	Mat new_image = extremifyImage(recolorImage(image, 0, 2.0), 128);
	//imwrite("ext.bmp", new_image);

	int *coords = (int *)calloc(2, sizeof(int));
	coords[0] = -1;
	coords[1] = -1;
	float lowest = 1.0;
	char **visited = (char **)calloc(image.rows, sizeof(char *));
	for (int i = 0; i < image.rows; i++) {
		visited[i] = (char *)calloc(image.cols, sizeof(char));
	}
	for (int y = 100; y < image.rows / 3; y++) {
		for (int x = 100; x < image.cols / 2; x++) {
			if (new_image.at<Vec3b>(y, x)[0] == 255 && new_image.at<Vec3b>(y, x)[1] == 255 && new_image.at<Vec3b>(y, x)[2] == 255 && !visited[y][x]) {
				visited[y][x] = 1;
				int *c = getBlob(new_image, x, y, visited);
				int dx = c[2] - c[0];
				int dy = c[3] - c[1];
				if (dy < 4 || dx < 4) {
					continue;
				}
				Mat other = subImage(image, c[0], c[1], c[2], c[3]);
				float error = compareImage(other, base);
				//printf("%d %d %d %d %f\n", c[0], c[1], c[2], c[3], error);
				if (error < 0.05) {
					lowest = error;
					coords[0] = c[0];
					coords[1] = c[1];
					return coords;
				}
			}
		}
	}
	//cout << "LOWEST: " << lowest << endl;
	return coords;
}

char *identifyChampion(Mat image) {
	display(image);
	char *championName = "";
	float total_error = 1.0;
	for (int i = 0; i < CHAMPIONS_LENGTH; i++) {
		float error = compareImage(CHAMPIONS[i], image);
		//printf("CHAMPION: %s, ERROR: %f\n", CHAMPIONS_NAME[i], error);
		if (error < total_error) {
			total_error = error;
			championName = CHAMPIONS_NAME[i];
		}
	}
	return championName;
}

char *identifyItem(Mat image) {
	char *itemName = "";
	float total_error = 1.0;
	for (int i = 0; i < ITEMS_LENGTH; i++) {
		float error = compareImage(ITEMS[i], image);
		if (error < total_error) {
			total_error = error;
			itemName = ITEMS_NAME[i];
		}
	}
	return itemName;
}

char *identifySummoner(Mat image) {
	char *summonerName = "";
	float total_error = 1.0;
	for (int i = 0; i < SUMMONERS_LENGTH; i++) {
		float error = compareImage(SUMMONERS[i], image);
		if (error < total_error) {
			total_error = error;
			summonerName = SUMMONERS_NAME[i];
		}
	}
	return summonerName;
}
