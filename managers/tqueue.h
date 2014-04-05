#ifndef __tqueue_h__
#define __tqueue_h__

#include <stdlib.h>			/* malloc */
#include <pthread.h>
/* #include <stdio.h> */	/* debug only */

typedef struct tqueue_ {
	void **data;
	int size;
	int capacity;
	int start;
	pthread_mutex_t lock;
	pthread_cond_t cond;
	int lock_error;
} tqueue;

int tqueue_init(tqueue *queue);
void tqueue_enqueue(tqueue *queue, void *data);
void *tqueue_dequeue(tqueue *queue);
void tqueue_clean(tqueue *queue);
void tqueue_destroy(tqueue *queue);

void tqueue_expand(tqueue *queue, int new_capacity);
int tqueue_lock(tqueue *queue);
int tqueue_unlock(tqueue *queue);
int tqueue_wait(tqueue *queue);
int tqueue_signal(tqueue *queue);

#endif
