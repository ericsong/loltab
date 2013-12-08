#ifndef __ArrayList_hpp__
#define __ArrayList_hpp__

#include <stdlib.h>

typedef struct ArrayList ArrayList;
ArrayList *CreateList();
void ALInsert(ArrayList *list, long data);
long ALGet(ArrayList *list, unsigned long index);
void DestroyList(ArrayList *list);

struct ArrayList {
	long *data;
	unsigned long size;
	unsigned long length;
};

#endif
