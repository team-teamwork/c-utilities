#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <assert.h>
#include "logger.h"
#include "queue.h"

typedef struct element element;
static element *element_create(const void *value, const size_t value_size);

/** 
 * Internal struct for element in the queue. It abstracts
 * away the linked-ness of the list from core operations
 * and it being internal affords users of the queue 
 * interface to only care about the values passed into
 * the queue.
 */
typedef struct element {
    void *value;
    struct element *next;
    struct element *previous;
} element;

/** 
 * Thread-safe implementation of queue.h. Is by definition
 * of the interface homogenous, by implication. No explicit
 * checks are made but free'ing the structure will be
 * impossible if this limitation is not adhered to.
 *
 * Will crash your program if memory cannot be allocated
 * and inform you of the error if that is the case.
 *
 * Will also set free'd pointers to NULL so that any dereferences
 * of dangling pointer causes your program to crash.
 */
typedef struct queue {
    element *head;
    element *tail;

    /* Lock for all read/write operations */
    pthread_mutex_t *mutex;

    /* Condition variable for whether or not the queue is empty */
    /* Negation in variable names is poor practice but it is the */
    /* only solution here considering the conditional variable API */
    /* (signal/broadcast) */
    pthread_cond_t *not_empty;

    /* Flag used to gauge when to wait on the not_empty condition variable */
    bool is_empty;

    /* A flag to set whenever the queue is about to be destroyed */
    bool cancel;
} queue;

queue *queue_create(const void *value, const size_t value_size) {
    assert(value);
    queue *q = queue_create_empty();
    
    pthread_mutex_lock(q->mutex);

    q->head = element_create(value, value_size);
    q->tail = q->head;
    
    q->cancel = false;
    q->is_empty = false;

    pthread_mutex_unlock(q->mutex);

    return q;
}

queue *queue_create_empty(void) {
    queue *q = calloc(1, sizeof(queue));
    q->mutex = calloc(1, sizeof(pthread_mutex_t));
    q->not_empty = calloc(1, sizeof(pthread_cond_t));

    if (!q) { 
        log_error("Could not allocate memory for queue");
        exit(1);
    }

    pthread_mutex_init(q->mutex, NULL);

    pthread_mutex_lock(q->mutex);

    pthread_cond_init(q->not_empty, NULL);

    q->head = NULL;
    q->tail = q->head;

    q->cancel = false;
    q->is_empty = true;

    pthread_mutex_unlock(q->mutex);

    return q;
}


/** 
 * Internal function for creating elements, containing a specified
 * value, to put inside the queue.
 *
 * Note: Memory is allocated for the element and must be freed sometime.
 *
 * @value: the value to store in the element. May not be null.
 *
 * Returns: A new element which may be added to a queue containing
 *          the supplied value. The returned element has no next
 *          or previous references, these are set to NULL.
 */
static element *element_create(const void *value, const size_t value_size) {
    assert(value);

    /* Allocate memory for a new element */
    element *e = calloc(1, sizeof(element));
    void *v = calloc(1, value_size);
    memcpy(v, value, value_size);

    if (!e) {
        log_error("Could not allocate memory for element in queue");
        exit(1);
    }

    /* Store a reference to the given value in the new element */
    e->value = v;
    e->next = NULL;
    e->previous = NULL;

    return e;
}

void queue_enqueue(queue *q, const void *value, const size_t value_size) {
    assert(q && value);

    pthread_mutex_lock(q->mutex);

    if (!q->cancel) {
        element *e = element_create(value, value_size);
    
        if (!q->head) {
            q->head = e;
            q->tail = q->head;
        } else {
            e->previous = q->tail;
            q->tail->next = e;
            q->tail = e;
        }
    
        q->is_empty = false;

        /* Ensure that threads waiting for the queue */
        /* to have elements are informed */
        pthread_cond_signal(q->not_empty);
    }

    pthread_mutex_unlock(q->mutex);
}

bool queue_is_empty(const queue *q) {
    assert(q);

    pthread_mutex_lock(q->mutex);
    bool ret = q->is_empty;
    pthread_mutex_unlock(q->mutex);

    /* The queue may actually be empty now */
    // TODO: How to avoid race condition?
    return ret;
}

void *queue_dequeue(queue *q) {
    assert(q);

    void *value = NULL;
    pthread_mutex_lock(q->mutex);
    
    /* We have a mutex-lock here, so we can atomically check this flag */
    if (q->is_empty) {
        /* We do not have to check the cancel flag here, if the thread is awoken */
        /* in a destruction context the waiting thread will be awoken, the later */
        /* if statement checks the flag before modification of the queue */

        /* This allows other threads to access the lock, thus signalling this thread */
        /* When this thread is awoken by this wait the queue will not be empty, or */
        /* the queue is about to be destroyed */
        pthread_cond_wait(q->not_empty, q->mutex);
    }

    /* We have a mutex lock again so we may atomically check both flags */
    if (!q->cancel && !q->is_empty) {
        value = q->head->value;
        if (q->head->next) {            
            q->head = q->head->next;
            free(q->head->previous);

            /* Make dereferencing dangling pointers crash the program */
            q->head->previous = NULL;
        } else {
            free(q->head);
            q->head = NULL;
            q->is_empty = true;
        }        
    }

    pthread_mutex_unlock(q->mutex);
    return value; 
}

void queue_destroy(queue *q, void (*freefunc)(void *value)) {
    assert(q);

    pthread_mutex_lock(q->mutex);
   
    q->cancel = true;

    while (q->head) {
        if (q->head->next) {            
            q->head = q->head->next;
            freefunc(q->head->previous->value);
            free(q->head->previous);
        
            /* Make dereferencing dangling pointers crash the program */
            q->head->previous = NULL;
        } else {
            freefunc(q->head->value);
            free(q->head);
            q->head = NULL;
        }        
    }

    /* The only conditional variable any thread may be waiting on is */
    /* the not_empty one, by broadcasting to all waiting threads on it */
    /* we may wake them. They should, before read and write operations */
    /* be checking the cancel flag hence this leads to the least convoluted */
    /* solution */
    pthread_cond_broadcast(q->not_empty);
    pthread_mutex_unlock(q->mutex);

    /* A condition variable can be destroyed immediately after all */
    /* the threads that are blocked on it are awakened */
    pthread_cond_destroy(q->not_empty);
    pthread_mutex_destroy(q->mutex);
    free(q->mutex);
    free(q->not_empty);
    free(q);
}

