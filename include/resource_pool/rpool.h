#ifndef __RESOURCE_POOL_H__
#define __RESOURCE_POOL_H__

#include "cqueue/cqueue.h"

/**
 * @brief macros for rpool
 * @def rp_true 1 status
 * @def rp_false 0 status
 * @def rp_success success status
 * @def rp_failure failure status
 * @def rp_resource_busy all resources busy error
 * @def rp_data_error corrupted data error
 */
#define rp_true 1
#define rp_false 0
#define rp_success 0
#define rp_failure -1
#define rp_resource_busy -2
#define rp_data_error -3

/**
 * @struct resource_stats structure
 */
typedef struct resource_stats {
	int idx;
	int free;
}resrc_stats;

/**
 * @struct resource_pool structure
 */
typedef struct resource_pool {
	/** resource count in this pool */
	int rcount;
	/** mutex for sync */
	pthread_mutex_t access;
	/** pointer to resource list, just for reference */
	void *resrc_list;
	/** status and info of resources */
	resrc_stats *rstat;
	/** cqueue of indexes of free resources */
	cqueue *free_ridx_cq;
}rpool;

#endif
