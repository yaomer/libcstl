/*
 * 用两个栈实现一个队列
 *
 * 思路很简单：
 * 我们一个栈(Q->push)用于入队，一个栈(Q->pop)用于出队
 * 当出队时Q->pop为空，我们就将Q->push中的元素全部压入到Q->pop中，
 * 然后正常出队即可
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../../common.h"
#include "../stack_array.h"
#include "queue.h"

Queue *
queue_init(void)
{
    Queue *Q;

    malloc_node(Q, Queue);
    Q->push = init();
    Q->pop = init();

    return Q;
}

void
queue_push(Queue *Q, int val)
{
    if (!isfull(Q->push))
        push(Q->push, val);
    else
        error("queue is full");
}

int
queue_pop(Queue *Q)
{
    if (isempty(Q->pop)) {
        while (!isempty(Q->push))
            push(Q->pop, pop(Q->push));
    }
    return pop(Q->pop);
}

void
queue_clear(Queue **Q)
{
    assert(Q);
    clear(&(*Q)->push);
    clear(&(*Q)->pop);
    free(*Q);
    *Q = NULL;
}
