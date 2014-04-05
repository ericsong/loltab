#include "tqueue.h"

int tqueue_init(tqueue *queue) {
	queue->size = 0;
	queue->capacity = 0;
	queue->data = NULL;
	queue->start = 0;
	queue->lock_error = pthread_mutex_init(&queue->lock, NULL);
	if (!queue->lock_error) {
		queue->lock_error = pthread_cond_init(&queue->cond, NULL);
	}
	return queue->lock_error;
}

void tqueue_enqueue(tqueue *queue, void *data) {
	if (queue->size == queue->capacity) {
		if (queue->data == NULL) {
			queue->data = (void **)calloc(1, sizeof(void *));
			queue->capacity = 1;
		} else {
			tqueue_expand(queue, queue->capacity * 2);
		}
	}
	queue->data[(queue->start + queue->size) % queue->capacity] = data;
	queue->size++;
}

void *tqueue_dequeue(tqueue *queue) {
	void *data;
	if (queue->size == 0) {
		return NULL;
	}
	data = queue->data[queue->start];
	queue->data[queue->start] = NULL;
	queue->start++;
	queue->start %= queue->capacity;
	queue->size--;
	return data;
}

void tqueue_clean(tqueue *queue) {
	int i;
	if (queue == NULL) {
		return;
	}
	if (queue->data == NULL) {
		return;
	}
	for (i = 0; i < queue->capacity; i++) {
		if (queue->data[i]) {
			free(queue->data[i]);
			queue->data[i] = NULL;
		}
	}
}

void tqueue_destroy(tqueue *queue) {
	if (queue == NULL) {
		return;
	}
	if (queue->data != NULL) {
		free(queue->data);
		queue->data = NULL;
	}
	queue->size = 0;
	queue->capacity = 0;
	queue->start = 0;
	if (!queue->lock_error) {
		pthread_mutex_destroy(&queue->lock);
		pthread_cond_destroy(&queue->cond);
	}
	queue->lock_error = 0;
}

void tqueue_expand(tqueue *queue, int new_capacity) {
	int i, index;
	void **temp;
	temp = (void **)calloc(new_capacity, sizeof(void *));
	for (i = 0; i < queue->size; i++) {
		index = (queue->start + i) % queue->size;
		temp[i] = queue->data[index];
	}
	if (queue->data != NULL) {
		free(queue->data);
	}
	queue->data = temp;
	queue->start = 0;
	queue->capacity = new_capacity;
}

int tqueue_lock(tqueue *queue) {
	if (!queue->lock_error) {
		return pthread_mutex_trylock(&queue->lock) == 0;
	}
	return 0;
}

int tqueue_unlock(tqueue *queue) {
	if (!queue->lock_error) {
		return pthread_mutex_unlock(&queue->lock) == 0;
	}
	return 0;
}

int tqueue_wait(tqueue *queue) {
	if (!queue->lock_error) {
		return pthread_cond_wait(&queue->cond, &queue->lock);
	}
	return 0;
}

int tqueue_signal(tqueue *queue) {
	if (!queue->lock_error) {
		return pthread_cond_signal(&queue->cond);
	}
	return 0;
}

