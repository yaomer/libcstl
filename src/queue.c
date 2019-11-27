#include <string.h>

#include "alloc.h"
#include "../include/list.h"
#include "../include/deque.h"

#define Q_LIST  1
#define Q_DEQUE 2

typedef struct __queue {
    void *queue;
    int adaptor;
} queue_t;

typedef void (*__queue_free_handler)(void *);

queue_t *queue_init(const char *name, __queue_free_handler qfree)
{
    queue_t *q = Calloc(1, sizeof(queue_t));
    if (name && strcmp(name, "list") == 0) {
        q->adaptor = Q_LIST;
        q->queue = list_init(NULL);
        list_set_free_handler(q->queue, qfree);
    } else {
        q->adaptor = Q_DEQUE;
        q->queue = deque_init_p();
        deque_set_free_handler(q->queue, qfree);
    }
    return q;
}

#define __do_queue(q, x, y) \
    switch ((q)->adaptor) { \
    case Q_LIST: x; break; \
    case Q_DEQUE: y; break; \
    }

void *queue_front(queue_t *q)
{
    __do_queue(q, return list_front(q->queue),
                  return deque_front_p(q->queue));
}

void *queue_back(queue_t *q)
{
    __do_queue(q, return list_back(q->queue),
                  return deque_back_p(q->queue));
}

bool queue_empty(queue_t *q)
{
    __do_queue(q, return list_empty(q->queue),
                  return deque_empty(q->queue));
}

size_t queue_size(queue_t *q)
{
    __do_queue(q, return list_size(q->queue),
                  return deque_size(q->queue));
}

void queue_push(queue_t *q, void *data)
{
    __do_queue(q, list_push_back(q->queue, data),
                  deque_push_back(q->queue, data));
}

void queue_pop(queue_t *q)
{
    __do_queue(q, list_pop_front(q->queue),
                  deque_pop_front(q->queue));
}

void queue_clear(queue_t *q)
{
    __do_queue(q, list_clear(q->queue),
                  deque_clear(q->queue));
}

void queue_free(queue_t *q)
{
    __do_queue(q, list_free(q->queue),
                  deque_free(q->queue));
    free(q);
}
