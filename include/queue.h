#ifndef _LIBCSTL_QUEUE_H
#define _LIBCSTL_QUEUE_H

#include <stddef.h>
#include <stdbool.h>

typedef struct __queue queue_t;

typedef void (*__queue_free_handler)(void *);

/* 初始化一个队列，name = [list, deque] */
queue_t *queue_init(const char *name/*=deque*/, __queue_free_handler qfree);
/* 访问队头元素 */
void *queue_front(queue_t *q);
/* 访问队尾元素 */
void *queue_back(queue_t *q);
/* 返回队列是否为空 */
bool queue_empty(queue_t *q);
/* 返回队列中存储的元素数 */
size_t queue_size(queue_t *q);
/* 入队操作 */
void queue_push(queue_t *q, void *data);
/* 出队操作 */
void queue_pop(queue_t *q);
/* 清空队列 */
void queue_clear(queue_t *q);
/* 释放一个队列 */
void queue_free(queue_t *q);

#endif // _LIBCSTL_QUEUE_H
