#include <stdbool.h>
#include <string.h>

#include "alloc.h"
#include "../include/vector.h"

/*
 * 我们这里用二叉堆来实现优先队列
 */

typedef bool (*__pq_comp_handler)(const void *, const void *);
typedef void (*__pq_free_handler)(void *);

typedef struct __pqueue {
    vector_t *buf;
    __pq_comp_handler pq_comp;
} pqueue_t;

#define __check_pq(pq) (assert(pq))

#define __parent(i) (i / 2)
#define __left(i)   (i * 2)
#define __right(i)  (i * 2 + 1)

static int __pq_comp(pqueue_t *pq, size_t i, size_t j)
{
    return pq->pq_comp(vector_entry(pq->buf, i), vector_entry(pq->buf, j));
}

static void __up(pqueue_t *pq, size_t i)
{
    size_t j = __parent(i);
    while (i > 1 && __pq_comp(pq, i, j)) {
        vector_swap(pq->buf, i, j);
        i = j;
        j = __parent(i);
    }
}

static void __down(pqueue_t *pq, size_t i)
{
    size_t l = __left(i);
    size_t r = __right(i);
    size_t size = vector_size(pq->buf) - 1;

    if (l > size) return;
    size_t o = __pq_comp(pq, l, i) ? l : i;
    if (r > size) goto next;
    if (__pq_comp(pq, r, o)) o = r;
next:
    if (o != i) {
        vector_swap(pq->buf, i, o);
        __down(pq, o);
    }
}

pqueue_t *pq_init(__pq_comp_handler pqcomp, __pq_free_handler pqfree)
{
    pqueue_t *pq = Calloc(1, sizeof(pqueue_t));
    pq->buf = vector_init(sizeof(void *));
    vector_set_free_handler(pq->buf, pqfree);
    vector_resize(pq->buf, 1); /* heap array下标要求从1开始 */
    pq->pq_comp = pqcomp;
    return pq;
}

bool pq_empty(pqueue_t *pq)
{
    __check_pq(pq);
    return vector_size(pq->buf) == 1;
}

size_t pq_size(pqueue_t *pq)
{
    __check_pq(pq);
    return vector_size(pq->buf) - 1;
}

void *pq_top(pqueue_t *pq)
{
    __check_pq(pq);
    return vector_entry(pq->buf, 1);
}

void pq_push(pqueue_t *pq, void *data)
{
    __check_pq(pq);
    vector_push_back(pq->buf, data);
    __up(pq, vector_size(pq->buf) - 1);
}

void pq_pop(pqueue_t *pq)
{
    __check_pq(pq);
    if (vector_size(pq->buf) < 2) return;
    vector_swap(pq->buf, 1, vector_size(pq->buf) - 1);
    vector_pop_back(pq->buf);
    __down(pq, 1);
}

void pq_free(pqueue_t *pq)
{
    __check_pq(pq);
    vector_free(pq->buf);
    free(pq);
}
