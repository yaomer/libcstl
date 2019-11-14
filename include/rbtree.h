#ifndef _LIBCSTL_RBTREE_H
#define _LIBCSTL_RBTREE_H

#include <stddef.h> /* for size_t */

typedef struct __rbtree rbtree_t;
typedef struct __rbtree_iterator * rbtree_iterator;
/* 用于比较树节点 */
typedef int (*__rbtree_comp_handler)(const void *, const void *);
/* 用于释放存储的键和值 */
typedef void (*__rbtree_free_handler)(void *, void *);

/* 初始化一个红黑树 */
rbtree_t *rbtree_init(__rbtree_comp_handler rcomp, __rbtree_free_handler rfree);
/* 返回指向最小节点的迭代器 */
rbtree_iterator rbtree_begin(rbtree_t *rbt);
/* 返回指向最大节点的迭代器 */
rbtree_iterator rbtree_end(rbtree_t *rbt);
/* 返回指向下一个节点的迭代器，如果iter为空或者下一个节点为空，则返回0；否则返回1 */
int rbtree_next(rbtree_iterator iter);
/* 返回指向上一个节点的迭代器，如果iter为空或者上一个节点为空，则返回0；否则返回1 */
int rbtree_prev(rbtree_iterator iter);
/* 取出迭代器中的键 */
void *rbtree_get_key(rbtree_iterator iter);
/* 取出迭代器中的值 */
void *rbtree_get_value(rbtree_iterator iter);
/* 释放一个迭代器 */
void rbtree_free_iterator(rbtree_iterator iter);
/* 如果树为空，则返回1；否则返回0 */
int rbtree_empty(rbtree_t *rbt);
/* 返回树的大小 */
size_t rbtree_size(rbtree_t *rbt);
/* 在树中查找指定节点，并返回其迭代器 */
rbtree_iterator rbtree_find(rbtree_t *rbt, const void *key);
/* 在树中插入一个节点 */
void rbtree_insert(rbtree_t *rbt, void *key, void *value);
/* 从树中擦除一个节点 */
void rbtree_erase(rbtree_t *rbt, const void *key);
/* 返回键值大于等于key的最小节点 */
rbtree_iterator rbtree_lower_bound(rbtree_t *rbt, const void *key);
/* 返回键值小于等于key的最大节点 */
rbtree_iterator rbtree_upper_bound(rbtree_t *rbt, const void *key);
/* 清空树 */
void rbtree_clear(rbtree_t *rbt);
/* 释放一个树 */
void rbtree_free(rbtree_t *rbt);

#endif // _LIBCSTL_RBTREE_H
