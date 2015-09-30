#include <stdbool.h>

/**
 * A (homogenous) queue interface. Implementations
 * are not required to check for homogenity but
 * free'ing the structure will be impossible if 
 * this limitation is not adhered to by virtue
 * of the destroy function design. See queue_destroy()
 */

#ifndef __QUEUE_H__
#define __QUEUE_H__

typedef struct queue queue;

/**
 * Creates a queue with the specified value. No
 * memory is allocated for the value, the caller
 * is expected to have allocated the appropriate
 * memory beforehand. 
 *
 * Note: The corresponding destroy function will
 * deallocate all memory used by each value pointer
 * still referenced by the queue. The queue should
 * only be freed through the corresponding 
 * destroy function, see queue_destroy();
 *
 * It is the responsibility of the caller to ensure
 * that each entry in the queue is of the same type.
 * See queue_destroy.
 *
 *
 * Invariant: queue_is_empty(create_queue(some_val)) == false
 *
 * Return: A queue with one element.
 */
queue *queue_create(const void *value, const size_t value_size);

/**
 * Creates an empty queue containing no value.
 *
 * Invariant: queue_is_empty(queue_create_empty()) == true
 *
 * Returns: An empty queue, with no elements
 */
queue *queue_create_empty(void);

/**
 * Adds a new element to the back of the queue. The value
 * passed will be copied, so any items put into the queue
 * has to be manually freed by the caller.
 *
 * Note: It is the responsibility of the caller to ensure
 *       that each entry in the queue is of the same type.
 *       See queue_destroy. 
 *
 * @q: The queue to add the value to. Must not be null.
 * @value: The value to add to the queue. Must not be null.
 */
void queue_enqueue(queue *q, const void *value, const size_t value_size);

/**
 * Check if the queue is empty.
 */
bool queue_is_empty(const queue *q);

/**
 * Gets the first value in the queue. The value offered
 * will have to be freed by the receiver.
 *
 * @q: The queue from which the value is to be retrieved.
 */
void *queue_dequeue(queue *q);

/* Destroys the queue, frees all memory that has been
 * allocated, additionally all memory used by each
 * value pointer still referenced by the queue will
 * be freed.
 * 
 * @q: The queue to free
 * @freefunc: Function specifying how to free values in
 *            the queue. Implies a homogenous queue.
 */
void queue_destroy(queue *q, void (*freefunc)(void *value));

#endif
