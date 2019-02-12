/**
 * @file rpool.c
 * @author Ravi Kant Pandey
 * @brief Resource pool file
 *
 * Provides data structure and functions for resource pool.
 */

#include <stdio.h>
#include <pthread.h>
#include <malloc.h>

#include "resource_pool/rpool.h"

/**
 * @brief function to initialize resource pool
 *
 * Initializes a resource pool instance with the provided list of resources.
 * This assumes that the resources are
 */
rpool* init_resource_pool(int rcount, void *resrc_list) {
	if (rcount <= 0) {
		printf("resource count %d inval\n", rcount);
		return NULL;
	}

	int i = 0;
	rpool *rp = malloc(sizeof(rpool));
	rp->rcount = rcount;

	rp->rstat = calloc(rcount, sizeof(resrc_stats));
	rp->free_ridx_cq = cqueue_init(rcount);

	for (i = 0; i < rcount; i++){
		rp->rstat[i].idx = i;
		rp->rstat[i].free = rp_true;
		cqueue_add(rp->free_ridx_cq, &rp->rstat[i]);
	}
	pthread_mutex_init(&rp->access, NULL);
	return rp;
}

int destroy_resource_pool(rpool *rp) {
	if (NULL == rp) {
		return rp_failure;
	}
	pthread_mutex_lock(&rp->access);
	if (rp) {
		if (rp->rstat) {
			free(rp->rstat);
			rp->rstat = NULL;
		}
		cqueue_destroy(rp->free_ridx_cq);
		rp->free_ridx_cq = NULL;
		pthread_mutex_unlock(&rp->access);
		pthread_mutex_destroy(&rp->access);
		free(rp);
	}
}

int take_resource(rpool *rp) {
	if (rp == NULL){
		return rp_failure;
	}

	pthread_mutex_lock(&rp->access);
	resrc_stats *rstat = (resrc_stats *)cqueue_remove(rp->free_ridx_cq);
	if (rstat == NULL) {
		return rp_resource_busy;
	}
	if (rstat->idx >= rp->rcount || rstat->free == rp_false) {
		return rp_data_error;
	}
	rstat->free = rp_false;
	pthread_mutex_unlock(&rp->access);
	return rstat->idx;
}

int return_resource(rpool *rp, int idx) {
	if (rp == NULL || idx >= rp->rcount){
		return rp_failure;
	}

	pthread_mutex_lock(&rp->access);
	if (rp->rstat[idx].free == rp_true) {
		return rp_success;
	}
	rp->rstat[idx].free = rp_true;
	int status = cqueue_add(rp->free_ridx_cq, &rp->rstat[idx]);
	if (status == -1) {
		return rp_data_error;
	}
	pthread_mutex_unlock(&rp->access);
}

#ifdef RPOOL_UNIT_TEST
int main() {
	int a[] = {1,2};
	int size = 2;

	rpool *rp = init_resource_pool(size, a);
	int id1 = take_resource(rp);
	printf("%d\n", id1);

	int id2 = take_resource(rp);
	printf("%d\n", id2);

	int id3 = take_resource(rp);
	if (id3 == rp_resource_busy) {
		printf("all busy\n");
	}
	return_resource(rp, id1);

	id2 = take_resource(rp);
	printf("%d\n", id2);

	destroy_resource_pool(rp);
	return 0;
}
#endif
