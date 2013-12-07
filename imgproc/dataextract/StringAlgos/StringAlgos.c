#include "StringAlgos.h"

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
	int i;
	for (i = beg; i < end; i++) {
		new_s[i - beg] = s[i];
	}
	new_s[end - beg] = '\0';
	return new_s;
}

int lenStr(char *str) {
	int length = 0;
	while (str[length] != '\0') {
		length++;
	}
	return length;
}

char *copyStr(char *str) {
	int length = lenStr(str), i;
	char *new_str = (char *)calloc(length + 1, sizeof(char));
	for (i = 0; i < length; i++) {
		new_str[i] = str[i];
	}
	new_str[length] = '\0';
	return new_str;
}
