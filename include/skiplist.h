#ifndef _LIBCSTL_SKIPLIST_H
#define _LIBCSTL_SKIPLIST_H

#include <stddef.h> /* for size_t */
#include <stdbool.h>

typedef struct __skiplist skiplist_t;
typedef struct __sl_iterator * sl_iterator;
/* 用于比较两个节点 */
typedef int (*__skiplist_comp_handler)(const void *, const void *);
/* 用于释放一个节点的键和值 */
typedef void (*__skiplist_free_handler)(void *, void *);

/* 初始化一个跳表 */
skiplist_t *sl_init(__skiplist_comp_handler slcomp, __skiplist_free_handler slfree);
/* 返回最小迭代器 */
sl_iterator sl_begin(skiplist_t *sl);
/* 返回最大迭代器 */
sl_iterator sl_end(skiplist_t *sl);
/* 返回iter的下一个迭代器，如果iter为空或下一个迭代器为空，则返回0 */
bool sl_next(sl_iterator iter);
/* 返回iter的上一个迭代器，如果iter为空或上一个迭代器为空，则返回0 */
bool sl_prev(sl_iterator iter);
/* if iter1 == iter2, return true */
bool sl_equal(sl_iterator iter1, sl_iterator iter2);
/* 取出迭代器中的键 */
void *sl_get_key(sl_iterator iter);
/* 取出迭代器中的值 */
void *sl_get_value(sl_iterator iter);
/* 释放一个迭代器 */
void sl_free_iterator(sl_iterator iter);
/* 查找指定的键 */
sl_iterator sl_find(skiplist_t *sl, const void *key);
/* 返回key的排名 */
size_t sl_order_of_key(skiplist_t *sl, const void *key);
/* 返回第order名的节点 */
sl_iterator sl_find_by_order(skiplist_t *sl, size_t order);
/* 插入一个键值对 */
void sl_insert(skiplist_t *sl, void *key, void *value);
/* 擦除键为key的节点 */
void sl_erase(skiplist_t *sl, const void *key);
/* 返回跳表是否为空 */
bool sl_empty(skiplist_t *sl);
/* 返回跳表的大小 */
size_t sl_size(skiplist_t *sl);
/* 返回键值大于等于key的最小节点 */
sl_iterator sl_lower_bound(skiplist_t *sl, const void *key);
/* 返回键值小于等于key的最大节点 */
sl_iterator sl_upper_bound(skiplist_t *sl, const void *key);
/* 清空跳表 */
void sl_clear(skiplist_t *sl);
/* 释放一个跳表 */
void sl_free(skiplist_t *sl);

#endif // _LIBCSTL_SKIPLIST_H
