/** @file cqueue.h
 */
#ifndef __CQUEUE_H__
#define __CQUEUE_H__

/**
 * @brief enum for cqueue return status
 */
typedef enum {
	//@{
	/**  success status */
	CQUEUE_SUCCESS = 0,
	//@}
	//@{
	/** error status (non-zero) */
	E_CQUEUE_NULL = 1,
	E_CQUEUE_EMPTY,
	E_CQUEUE_FULL,
	E_CQUEUE_MEMORY,
	E_CQUEUE_INVAL,
	//@}
}cqueue_status;

/**
 * @brief structure to create var of circular queue.
 *
 */
typedef struct cqueue {
	/** size represents size of cqueue*/
	int size;
	/** the data array */
	int **value;
	/** front of cqueue */
	int front;
	/** rear of cqueue */
	int rear;
	/** mutex var for synchronizaton */
	pthread_mutex_t access;
}cqueue;

/**
 * @brief function to add data in circular queue
 *
 * This function adds data to provided cqueue pointer. The addition property
 * performs it in thread-safe way. If two threads try to add data together to
 * the same cqueue, the function will manage without corrupting the data.
 * If the cqueue becomes full, no new data shall be added.
 * @param q cqueue pointer
 * @param data the data value to be added
 * @return cqueue_status
 * @see cqueue_status
 */
int cqueue_add(cqueue *q, void *data);

/**
 * @brief function to remove data from circular queue
 *
 * This function removes from provided cqueue pointer. The deletion property
 * performs it in thread-safe way. If two threads try to add remove together
 * from the same cqueue, the function will manage without corrupting the data.
 * @param q cqueue pointer from which data to be removed
 * @return data retrived value is to be returned or NULL in case of any error.
 */
void *cqueue_remove(cqueue *q);

/**
 * @brief function to initialize cqueue
 *
 * Initializes cqueue var with size value.
 * @param size
 * @return @a cqueue pointer through which data can be accessed.
 * @see cqueue_add()
 * @see cqueue_remove()
 * @see cqueue_destroy()
 */
cqueue *cqueue_init(int size);

/**
 * @brief function to delete cqueue
 *
 * Deletes the context for cqueue var and resets the value.
 * @param cq cqueue pointer to be deleted.
 * @return status
 */
int cqueue_destroy(cqueue *cq);

#endif
