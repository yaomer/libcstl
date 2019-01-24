#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "../common.h"
#include "queue.h"

Queue *
queue_init(void)
{
    Queue *p;

    malloc_node(p, Queue);
    p->front = p->rear = NULL;

    return p;
}

void
queue_push(Queue *p, char *s)
{
    _Queue *x;

    malloc_node(x, _Queue);
    malloc_str(x->s, s);

    if (is_queue_empty(p))
        p->front = p->rear = x;
    else {
        p->rear->next = x;
        p->rear = x;
    }
    x->next = NULL;
}

void
queue_pop(Queue *p)
{
    _Queue *tmp;

    if (!is_queue_empty(p)) {
        tmp = p->front->next;
        free(p->front->s);
        free(p->front);
        if (!(p->front = tmp))   /* 队列为空 */
            p->rear = NULL;
    } else
        error("queue is empty");
}

_Queue *
queue_top(Queue *p)
{
    if (!is_queue_empty(p))
        return p->front;
    else
        error("queue is empty");
}

int
is_queue_empty(Queue *p)
{
    assert(p);
    return p->front == NULL;
}

void
queue_clear(Queue **p)
{
    assert(p);
    while (!is_queue_empty(*p))
        queue_pop(*p);
    free(*p);
    *p = NULL;
}
