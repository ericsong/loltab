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

char *append(char *str, char c) {
	char *new_str = (char *)calloc(lenStr(str) + 1, sizeof(char));
	int i;
	for (i = 0; str[i] != '\0'; i++) {
		new_str[i] = str[i];
	}
	new_str[i++] = c;
	new_str[i] = '\0';
	return new_str;
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

int strEquals(char *c1, char *c2) {
	if (lenStr(c1) != lenStr(c2)) {
		return 0;
	}
	for (int i = 0; i < lenStr(c1); i++) {
		if (c1[i] != c2[i]) {
			return 0;
		}
	}
	return 1;
}

int indexOf(char *str, char c) {
	int length = lenStr(str);
	for (int i = 0; i < length; i++) {
		if (str[i] == c) {
			return i;
		}
	}
	return -1;
}
