#ifndef __StringAlgos_hpp__
#define __StringAlgos_hpp__

#include <stdlib.h>

char *subStr(char *str, int beg, int end);
char *append(char *str, char c);
char *concatStr(char *str1, char *str2);
int lenStr(char *str);
char *copyStr(char *str);
int strEquals(char *str1, char *str2);
int indexOf(char *str, char c);

#endif
