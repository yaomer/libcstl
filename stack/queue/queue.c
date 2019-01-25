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
    assert(Q);
    if (!isfull(Q->push))
        push(Q->push, val);
    else
        error("queue is full");
}

int
queue_pop(Queue *Q)
{
    assert(Q);
    if (isempty(Q->pop)) {
        while (!isempty(Q->push))
            push(Q->pop, pop(Q->push));
    }

    if (!isempty(Q->pop))
        return pop(Q->pop);
    else
        error("queue is isempty");
}

/*
 * 只要Q->push满了，队列Q就满了，不论Q->pop是否满了
 * 因为queue_push只针对Q->push
 */
int
is_queue_full(Queue *Q)
{
    assert(Q);
    return isfull(Q->push);
}

/*
 * 只要Q->push和Q->pop中有一个不为空，队列Q就不为空
 */
int
is_queue_empty(Queue *Q)
{
    assert(Q);
    return isempty(Q->push) && isempty(Q->pop);
}

void
queue_clear(Queue **Q)
{
    assert(Q && *Q);
    clear(&(*Q)->push);
    clear(&(*Q)->pop);
    free(*Q);
    *Q = NULL;
}
