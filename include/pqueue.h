#ifndef _LIBCSTL_PQUEUE_H
#define _LIBCSTL_PQUEUE_H

#include <stddef.h> /* for size_t */
#include <stdbool.h>

typedef struct __pqueue pqueue_t;

typedef bool (*__pqueue_comp_handler)(const void *, const void *);
/*
 * bool comp(const void *l, const void *r)
 * {
 *     // return *(int*)l > *(int*)r; ==> 最大优先队列
 *     // return *(int*)l < *(int*)r; ==> 最小优先队列
 * }
 */
typedef void (*__pqueue_free_handler)(void *);

/* 初始化一个优先队列 */
pqueue_t *pqueue_init(__pqueue_comp_handler pqcomp, __pqueue_free_handler pqfree);
/* 判断优先队列是否为空 */
bool pqueue_empty(pqueue_t *pq);
/* 返回优先队列中存储的元素数 */
size_t pqueue_size(pqueue_t *pq);
/* 返回堆顶元素 */
void *pqueue_top(pqueue_t *pq);
/* 插入一个元素 */
void pqueue_push(pqueue_t *pq, void *data);
/* 弹出堆顶元素 */
void pqueue_pop(pqueue_t *pq);
/* 释放一个优先队列 */
void pqueue_free(pqueue_t *pq);

#endif // _LIBCSTL_PQUEUE_H
