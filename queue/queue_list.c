#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../common.h"
#include "queue_list.h"

Queue *
queue_init(void)
{
    Queue *p;

    malloc_node(p, Queue);
    p->front = p->rear = NULL;

    return p;
}

void
queue_push(Queue *Q, int new_val)
{
    _Queue *p;

    malloc_node(p, _Queue);
    p->val = new_val;

    if (is_queue_empty(Q))   /* 初始化队列 */
        Q->front = Q->rear = p;
    else {   /* 插到队尾 */
        Q->rear->next = p;
        Q->rear = p;
    }
    p->next = NULL;
}

void
queue_pop(Queue *Q)
{
    _Queue *tmp;

    if (!is_queue_empty(Q)) {
        tmp = Q->front->next;
        free(Q->front);
        if (!(Q->front = tmp))   /* 队列为空 */
            Q->rear = NULL;
    } else
        error("queue is empty");
}

int
queue_top(Queue *Q)
{
    if (!is_queue_empty(Q))
        return Q->front->val;
    else
        error("queue is empty");
}

int
is_queue_full(Queue *Q)
{
    return 0;
}

int
is_queue_empty(Queue *Q)
{
    assert(Q);
    return Q->front == NULL;
}

void
queue_clear(Queue **Q)
{
    assert(Q);
    while (!is_queue_empty(*Q))
        queue_pop(*Q);
    free(*Q);
    *Q = NULL;
}
