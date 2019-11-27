#ifndef _LIBCSTL_DEQUE_H
#define _LIBCSTL_DEQUE_H

#include <stddef.h> /* for size_t */
#include <stdbool.h>

typedef struct __deque deque_t;
typedef struct __deque_iterator * deque_iterator;
/* 如果存储的数据分配于堆上，则需要提供释放函数 */
typedef void (*__deque_free_handler)(void *);

void deque_set_free_handler(deque_t *d, __deque_free_handler dfree);

/* 初始化一个deque */
deque_t *deque_init(size_t typesize);
/* 初始化一个deque，存储的是指向数据的指针
 * 如果以此函数初始化，则需要在访问元素时使用_p后缀的函数 */
deque_t *deque_init_p(void);
/* 访问首元素 */
void *deque_front(deque_t *d);
void *deque_front_p(deque_t *d);
/* 访问尾元素 */
void *deque_back(deque_t *d);
void *deque_back_p(deque_t *d);
/* 返回指向首元素的迭代器 */
deque_iterator deque_begin(deque_t *d);
/* 返回指向尾元素的迭代器 */
deque_iterator deque_end(deque_t *d);
/* 返回下一个元素的迭代器，如果成功，返回真 */
bool deque_next(deque_iterator iter);
/* 返回上一个元素的迭代器，如果成功，返回真 */
bool deque_prev(deque_iterator iter);
/* 取出迭代器中的数据 */
void *deque_get(deque_iterator iter);
void *deque_get_p(deque_iterator iter);
/* 释放一个迭代器 */
void deque_free_iterator(deque_iterator iter);
/* 根据下标访问指定元素 */
void *deque_entry(deque_t *d, size_t index);
void *deque_entry_p(deque_t *d, size_t index);
/* 返回deque是否为空 */
bool deque_empty(deque_t *d);
/* 返回deque中存储的元素数量 */
size_t deque_size(deque_t *d);
/* 释放未使用的内存 */
void deque_shrink_to_fit(deque_t *d);
/* 在deque后端插入一个元素 */
void deque_push_back(deque_t *d, const void *data);
/* 在deque后端弹出一个元素 */
void deque_pop_back(deque_t *d);
/* 在deque前端插入一个元素 */
void deque_push_front(deque_t *d, const void *data);
/* 在deque前端弹出一个元素 */
void deque_pop_front(deque_t *d);
/* 更改deque中容纳的元素数 */
void deque_resize(deque_t *d, size_t count);
/* 交换两个元素 */
void deque_swap(deque_t *d, size_t i, size_t j);
/* 清空deque */
void deque_clear(deque_t *d);
/* 释放deque */
void deque_free(deque_t *d);

#endif // _LIBCSTL_DEQUE_H
