#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <pthread.h>

#include "cqueue/cqueue.h"

/**
 * @brief function to add data in circular queue
 *
 * pthread mutex have been used for synchronization.
 */
int cqueue_add(cqueue *q, void* data) {
	if (NULL == q || NULL == q->value) {
		return E_CQUEUE_NULL;
	}
	pthread_mutex_lock(&q->access);
	if (q->front == (q->rear+1) % q->size){
		pthread_mutex_unlock(&q->access);
		return E_CQUEUE_FULL;
	}
	if (q->rear == -1) {
		q->front = q->rear = 0;
	} else {
		q->rear = (q->rear + 1) % q->size;
	}
	q->value[q->rear] = data;
	pthread_mutex_unlock(&q->access);
	return CQUEUE_SUCCESS;
}

/**
 * @brief function to remove data from circular queue
 *
 * pthread mutex have been used for synchronization.
 */
void *cqueue_remove(cqueue *q) {
	void *data = NULL;
	if (NULL == q || NULL == q->value) {
		return NULL;
	}
	pthread_mutex_lock(&q->access);
	if (q->front == -1) {
		pthread_mutex_unlock(&q->access);
		return NULL;
	}
	data = q->value[q->front];
	if (q->front == q->rear) {
		q->front = q->rear = -1;
	} else {
		q->front = (q->front+1) % q->size;
	}
	pthread_mutex_unlock(&q->access);
	return data;
}

/**
 * @brief function to initialize cqueue
 *
 */
cqueue *cqueue_init(int size) {
	cqueue *q = (cqueue *)malloc(sizeof(cqueue));

	if (NULL == q) {
		return NULL;
	}
	q->size = size;
	q->value = (int **)malloc(sizeof(int *) * size);
	q->rear = q->front = -1;

	pthread_mutex_init(&q->access, NULL);
	return q;
}

/**
 * @brief function to delete cqueue
 *
 */
int cqueue_destroy(cqueue *cq) {
	if (cq == NULL) {
		return E_CQUEUE_EMPTY;
	}

	pthread_mutex_lock(&cq->access);
	if (cq->value) {
		free(cq->value);
		cq->value = NULL;
	}
	pthread_mutex_unlock(&cq->access);
	free(cq);
	cq = NULL;
	return CQUEUE_SUCCESS;
}
