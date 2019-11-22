#include <stdbool.h>
#include <string.h>

#include "alloc.h"
#include "../include/vector.h"

/*
 * 我们这里用二叉堆来实现优先队列
 */

typedef bool (*__pqueue_comp_handler)(const void *, const void *);
typedef void (*__pqueue_free_handler)(void *);

typedef struct __pqueue {
    vector_t *buf;
    __pqueue_comp_handler pq_comp;
} pqueue_t;

#define __check_pqueue(pq) (assert(pq))

#define __parent(i) (i / 2)
#define __left(i)   (i * 2)
#define __right(i)  (i * 2 + 1)

static void __copy(void *des, const void *src)
{
    memcpy(des, &src, sizeof(const void *));
}

static void * __pqueue_get_data(pqueue_t *pq, size_t index)
{
    return *(void**)vector_entry(pq->buf, index);
}

static bool __pqueue_comp(pqueue_t *pq, size_t i, size_t j)
{
    return pq->pq_comp(__pqueue_get_data(pq, i), __pqueue_get_data(pq, j));
}

/* 维护堆的性质 */
static void __heap(pqueue_t *pq, size_t i)
{
    size_t l = __left(i);
    size_t r = __right(i);

    if (l > vector_size(pq->buf)) return;
    size_t o = __pqueue_comp(pq, l, i) ? l : i;
    if (r > vector_size(pq->buf)) goto next;
    if (__pqueue_comp(pq, r, o)) o = r;
next:
    if (o != i) {
        vector_swap(pq->buf, i, o);
        __heap(pq, o);
    }
}

pqueue_t *pqueue_init(__pqueue_comp_handler pqcomp, __pqueue_free_handler pqfree)
{
    pqueue_t *pq = Calloc(1, sizeof(pqueue_t));
    pq->buf = vector_init(sizeof(void *));
    vector_set_copy_handler(pq->buf, __copy);
    vector_set_free_handler(pq->buf, pqfree);
    vector_resize(pq->buf, 1); /* heap array下标要求从1开始 */
    pq->pq_comp = pqcomp;
    return pq;
}

bool pqueue_empty(pqueue_t *pq)
{
    __check_pqueue(pq);
    return vector_empty(pq->buf);
}

size_t pqueue_size(pqueue_t *pq)
{
    __check_pqueue(pq);
    return vector_size(pq->buf) - 1;
}

void *pqueue_top(pqueue_t *pq)
{
    __check_pqueue(pq);
    return __pqueue_get_data(pq, 1);
}

void pqueue_push(pqueue_t *pq, void *data)
{
    __check_pqueue(pq);
    vector_push_back(pq->buf, data);
    size_t i = vector_size(pq->buf) - 1;
    size_t j = __parent(i);
    while (i > 1 && __pqueue_comp(pq, i, j)) {
        vector_swap(pq->buf, i, j);
        i = j;
        j = __parent(i);
    }
}

void pqueue_pop(pqueue_t *pq)
{
    __check_pqueue(pq);
    if (vector_size(pq->buf) < 2) return;
    vector_swap(pq->buf, 1, vector_size(pq->buf) - 1);
    vector_pop_back(pq->buf);
    __heap(pq, 1);
    return;
}

void pqueue_free(pqueue_t *pq)
{
    __check_pqueue(pq);
    vector_free(pq->buf);
    free(pq);
}
