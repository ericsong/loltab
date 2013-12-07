#include "StringAlgos.hpp"

char *concatStr(char *str1, char *str2) {
	char *str3;
	int length1 = 0, length2 = 0, length = 0, i;
	for (i = 0; str1[i] != '\0'; i++) {
		length1++;
	}
	for (i = 0; str2[i] != '\0'; i++) {
		length2++;
	}
	length = length1 + length2 + 1;
	str3 = (char *)calloc(length, sizeof(char));
	for (i = 0; i < length1; i++) {
		str3[i] = str1[i];
	}
	for (i = 0; i < length2; i++) {
		str3[i + length1] = str2[i];
	}
	str3[length - 1] = '\0';
	return str3;
}

char *subStr(char *str, int beg, int end) {
	char *new_s = (char *)calloc(end - beg + 1, sizeof(char));
	int i;
	for (i = beg; i < end; i++) {
		new_s[i - beg] = str[i];
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
