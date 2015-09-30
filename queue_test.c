#include <stdlib.h>
#include <assert.h>
#include "queue.h"
#include "queue_test.h"

static void empty_queue_should_be_empty(void);
static void non_empty_queue_is_not_empty(void);
static void enqueue_to_queue_makes_it_non_empty(void);
static void enqueue_dequeue_cancels_out(void);
static void repeated_enque_deque_cancels_out(void);
static void dequeuing_all_items_will_result_in_empty_queue(void);
static void insertion_order_is_preserved(void);

void queue_tests(void) {
    empty_queue_should_be_empty();
    non_empty_queue_is_not_empty();
    enqueue_to_queue_makes_it_non_empty();
    enqueue_dequeue_cancels_out();
    repeated_enque_deque_cancels_out();
    dequeuing_all_items_will_result_in_empty_queue();
    insertion_order_is_preserved();
}

static void empty_queue_should_be_empty(void) {
    queue *empty_queue = queue_create_empty();
    assert(queue_is_empty(empty_queue));
    queue_destroy(empty_queue, free);
}

static void enqueue_to_queue_makes_it_non_empty(void) {
    int some_value = 5;

    queue *q = queue_create_empty();
    queue_enqueue(q, &some_value, sizeof(int));

    assert(!queue_is_empty(q));
    queue_destroy(q, free);
}

static void non_empty_queue_is_not_empty(void) {
    int some_value = 5;
    queue *q = queue_create(&some_value, sizeof(int));

    assert(!queue_is_empty(q));
    queue_destroy(q, free);
}

static void enqueue_dequeue_cancels_out(void) {
    int some_value = 5;
    queue *q = queue_create_empty();
    queue_enqueue(q, &some_value, sizeof(int));

    /* We do not care about the return value */
    free(queue_dequeue(q));

    assert(queue_is_empty(q));
    queue_destroy(q, free);
}

static void repeated_enque_deque_cancels_out(void) {
    queue *q = queue_create_empty();

    for (int i = 0; i < 10; i++) {
        queue_enqueue(q, &i, sizeof(int));

        /* We do not care about the return value */
        free(queue_dequeue(q));
    }

    assert(queue_is_empty(q));
    queue_destroy(q, free);

}

static void dequeuing_all_items_will_result_in_empty_queue(void) {
    queue *q = queue_create_empty();

    for (int i = 0; i < 10; i++) {
        queue_enqueue(q, &i, sizeof(int));
    }

    for (int i = 0; i < 10; i++) {
        /* We do not care about the return value */
        free(queue_dequeue(q));
    }

    assert(queue_is_empty(q));
    queue_destroy(q, free);
}

static void insertion_order_is_preserved(void) {
    queue *q = queue_create_empty();

    for (int i = 0; i < 10; i++) {
        queue_enqueue(q, &i, sizeof(int));
    }

    for (int i = 0; i < 10; i++) {
        int *head = queue_dequeue(q);
        assert(*head == i);
        free(head);
    }

    assert(queue_is_empty(q));
    queue_destroy(q, free);

}
