#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../common.h"
#include "queue_array.h"

/*
 * 简单的用数组实现的顺序队列很容易溢出，所以一般都实现为
 * 所谓的循环队列，即当head或tail指针到达数组末尾时就让
 * 它们又绕回到数组开头。
 *
 * 需要说明的一点是：这种循环队列并不常用，我们这里给出简单
 * 的实现，只是为了说明一些编程技巧。
 */

Queue *
queue_init(void)
{
    Queue *Q;

    malloc_node(Q, Queue);
    bzero(Q->val, sizeof(Q->val));
    Q->head = Q->tail = 0;

    return Q;
}

/*
 * 使用取余运算很容易实现回绕，例如判断队列是否已满：
 * head == (tail + 1) % MAXVAL
 */

void
queue_push(Queue *Q, int new_val)
{
    if (!is_queue_full(Q)) {
        Q->val[Q->tail] = new_val;
        Q->tail = (Q->tail + 1) % MAXVAL;
    } else
        error("queue is full");
}

int
queue_pop(Queue *Q)
{
    int tmp;

    if (!is_queue_empty(Q)) {
        tmp = Q->val[Q->head];
        Q->head = (Q->head + 1) % MAXVAL;
        return tmp;
    } else
        error("queue is empty");
}

int
queue_top(Queue *Q)
{
    if (!is_queue_empty(Q))
        return Q->val[Q->head];
    else
        error("queue is empty");
}

void
queue_clear(Queue **Q)
{
    assert(Q);
    free(*Q);
    *Q = NULL;
}

/*
 *               head
 * [ 1 ][ 5 ][  ][ 3 ][ 7 ]
 *           tail
 * 上图这个队列就已经满了，而且很显然再入队一次后，tail就和head重合了
 */
int is_queue_full(Queue *Q)
{
    assert(Q);
    return Q->head == (Q->tail + 1) % MAXVAL;
}

int is_queue_empty(Queue *Q)
{
    assert(Q);
    return Q->head == Q->tail;
}
