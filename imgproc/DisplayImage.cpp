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
char getNext(char);
Mat createImage(int, int, int);
Mat subImage(Mat, int, int, int, int);
Mat resizeImage(Mat, int, int);
Mat recolorImage(Mat, int, float);
Mat extremifyImage(Mat, int);
double compareImage(Mat, Mat);
void blit(Mat, Mat, int, int);
double matchString(Mat, char *);
int *getBlob(Mat, int, int, char **);
int getNumber(Mat);

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
	int brightness = 0;
	float contrast = 1.5;
	/*cout << "Brightness: ";
	cin.getline(buffer, 100);
	brightness = atoi(buffer);
	cout << "Contrast: ";
	cin.getline(buffer, 100);
	contrast = atof(buffer);*/
	int x1 = 650, x2 = 700, y1 = 175, y2 = 540;
	/*cout << "Box (to be removed later):\n";
	cout << "x1: ";
	cin.getline(buffer, 100);
	x1 = atoi(buffer);
	cout << "y1: ";
	cin.getline(buffer, 100);
	y1 = atoi(buffer);
	cout << "x2: ";
	cin.getline(buffer, 100);
	x2 = atoi(buffer);
	cout << "y2: ";
	cin.getline(buffer, 100);
	y2 = atoi(buffer);*/

	// change image
	Mat new_image = subImage(image, 0, 0, image.cols, image.rows);
	new_image = subImage(new_image, x1, y1, x2, y2);
	new_image = recolorImage(new_image, brightness, contrast);
	new_image = extremifyImage(new_image, 128);
	char fname[] = "00000000.bmp";
	int index = 0;
	char c = '1';
	char **visited = (char **)calloc(new_image.rows, sizeof(char *));
	for (int i = 0; i < new_image.rows; i++) {
		visited[i] = (char *)calloc(new_image.cols, sizeof(char));
	}
	for (int y = 0; y < new_image.rows; y++) {
		for (int x = 0; x < new_image.cols; x++) {
			if (!visited[y][x] && (
				new_image.at<Vec3b>(y, x)[0] == 255 ||
				new_image.at<Vec3b>(y, x)[1] == 255 ||
				new_image.at<Vec3b>(y, x)[2] == 255)) {
				int *coords = getBlob(new_image, x, y, visited);
				//printf("COORDS: %d %d %d %d\n", coords[0], coords[1], coords[2], coords[3]);
				int dx = coords[2] - coords[0];
				int dy = coords[3] - coords[1];
				if (dx > dy) {
					Mat Rnum = subImage(image, x1 + coords[0] - 1, y1 + coords[1] - 1, x1 + coords[2] + 1, y1 + coords[3] + 1);
					double ERR = matchString(Rnum, "08");
					printf("ERR: %f\n", ERR);
					int X_t = coords[0];
					coords[0] += dx / 2;
					Mat snum = subImage(image, x1 + X_t - 1, y1 + coords[1] - 1, x1 + coords[0] + 1, y1 + coords[3] + 1);
					//printf("Writing %s\n", fname);
					imwrite(fname, snum);
					int n = getNumber(snum);
					printf("Found %d in %s\n\n", n, fname);
					fname[index] = c;
					if (c == '9') {
						index++;
					}
					c = getNext(c);
				}
				Mat pnum = subImage(image, x1 + coords[0] - 1, y1 + coords[1] - 1, x1 + coords[2] + 1, y1 + coords[3] + 1);
				//printf("Writing %s\n", fname);
				imwrite(fname, pnum);
				int n = getNumber(pnum);
				printf("Found %d in %s\n\n", n, fname);
				fname[index] = c;
				if (c == '9') {
					index++;
				}
				c = getNext(c);
			}
			visited[y][x] = 1;
		}
	}
	return 1;
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

char getNext(char c) {
	if (c == '9') {
		c = '1';
	} else {
		c++;
	}
	return c;
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
	while (length < X->length) {
		// add to paired list
		y = ALGet(Y, length);
		x = ALGet(X, length);
		visited[y][x] = 1;
		if (image.at<Vec3b>(y, x)[0] == 255 ||
			image.at<Vec3b>(y, x)[1] == 255 ||
			image.at<Vec3b>(y, x)[2] == 255) { 
			if (x + 1 < image.cols && !visited[y][x + 1]) {
				ALInsert(X, x + 1);
				ALInsert(Y, y);
			}
			if (y + 1 < image.rows && !visited[y + 1][x]) {
				ALInsert(X, x);
				ALInsert(Y, y + 1);
			}
			if (x - 1 >= 0 && !visited[y][x - 1]) {
				ALInsert(X, x - 1);
				ALInsert(Y, y);
			}
			if (y - 1 >= 0 && !visited[y - 1][x]) {
				ALInsert(X, x);
				ALInsert(Y, y - 1);
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
	coords[2] = hx;
	coords[3] = hy;
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

double matchString(Mat image, char str[]) {
	// oh boy (use getNumber as inspiration)
	double error = 1.0;
	DIR *dp;
	struct dirent *dirItem;

	char directory[] = "../characters/?/";
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

	// build cache
	int numImages[length];
	int max_length = 0;
	for (i = 0; i < length; i++) {
		numImages[i] = 0;
		directory[14] = str[i];
		dp = opendir(directory);
		while ((dirItem = readdir(dp)) != NULL) {
			if (dirItem->d_name[0] == '.') {
				continue;
			}
			cout << dirItem->d_name << endl;
			numImages[i]++;
		}
		if (numImages[i] > max_length) {
			max_length = numImages[i];
		}
		closedir(dp);
	}
	Mat characters[length][max_length];
	for (i = 0; i < length; i++) {
		directory[14] = str[i];
		dp = opendir(directory);
		int index;
		for (index = 0; index < numImages[i]; index++) {
			dirItem = readdir(dp);
			while (dirItem->d_name[0] == '.') {
				dirItem = readdir(dp);
			}
			characters[i][index] = imread(concatStr(directory, dirItem->d_name), 1);
			characters[i][numImages[i] - 1] = resizeImage(	characters[i][numImages[i] - 1],
															characters[i][numImages[i] - 1].cols,
															image.rows);
		}
		closedir(dp);
	}
	// compute least error based on all the combinations
	int total_combinations = 1;
	for (i = 0; i < length; i++) {
		total_combinations *= numImages[i];
	}
	for (i = 0; i < total_combinations; i++) {
		double total_error = 1.0;
		int combination = i, total_width = 0, digit;
		for (digit = 0; digit < length; digit++) {
			total_width += characters[digit][combination % numImages[digit]].cols;
			combination /= numImages[digit];
		}
		combination = i;
		Mat word = createImage(total_width, image.rows, image.channels());
		int offset = 0;
		for (digit = 0; digit < length; digit++) {
			blit(word, characters[digit][combination % numImages[digit]], offset, 0);
			combination /= numImages[digit];
			offset += characters[digit][combination % numImages[digit]].cols;
		}
		word = resizeImage(word, image.cols, word.rows);
		total_error = compareImage(word, image);
		if (total_error < error) {
			error = total_error;
		}
	}
	return error;
}

int getNumber(Mat image) {
	double error = 1.0;
	int number = -1;
	DIR *dp;
	struct dirent *dirItem;
	char *directories[10] = {
		"../characters/0/",
		"../characters/1/",
		"../characters/2/",
		"../characters/3/",
		"../characters/4/",
		"../characters/5/",
		"../characters/6/",
		"../characters/7/",
		"../characters/8/",
		"../characters/9/"
	};
	for (int i = 0; i < 10; i++) {
		dp = opendir(directories[i]);
		double total_error = 1.0;
		int numberOfItems = 0;
		if (dp != NULL) {
			while ((dirItem = readdir(dp)) != NULL) {
				if (dirItem->d_name[0] == '.') {
					continue;
				}
				Mat new_image = resizeImage(imread(concatStr(directories[i], dirItem->d_name), 1), image.cols, image.rows);
				double temp_error = compareImage(image, new_image);
				if (total_error > temp_error) {
					total_error = temp_error;
				}
				numberOfItems++;
			}
			//printf("TOTAL ERROR [%d]: %f\n", i, total_error);
			if (numberOfItems > 0 && total_error < error) {
				error = total_error;
				number = i;
			}
			closedir(dp);
		}
	}
	return number;
}

char *findExpression(Mat image) {
	char *expression = (char *)malloc(sizeof(char));
	ArrayList 
}
