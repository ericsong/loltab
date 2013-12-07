#include "ArrayList.h"

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
