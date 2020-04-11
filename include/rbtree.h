#ifndef _LIBCSTL_RBTREE_H
#define _LIBCSTL_RBTREE_H

#include <stddef.h> /* for size_t */
#include <stdbool.h>

typedef struct __rbtree rbtree_t;
typedef struct __rb_iterator * rb_iterator;
/* 用于比较树节点 */
typedef int (*__rbtree_comp_handler)(const void *, const void *);
/* 用于释放存储的键和值 */
typedef void (*__rbtree_free_handler)(void *, void *);

/* 初始化一个红黑树 */
rbtree_t *rb_init(__rbtree_comp_handler rcomp, __rbtree_free_handler rfree);
/* 返回指向最小节点的迭代器 */
rb_iterator rb_begin(rbtree_t *rb);
/* 返回指向最大节点的迭代器 */
rb_iterator rb_end(rbtree_t *rb);
/* 返回指向下一个节点的迭代器，如果成功，则返回真 */
bool rb_next(rb_iterator iter);
/* 返回指向上一个节点的迭代器，如果成功，则返回真 */
bool rb_prev(rb_iterator iter);
/* 取出迭代器中的键 */
void *rb_get_key(rb_iterator iter);
/* 取出迭代器中的值 */
void *rb_get_value(rb_iterator iter);
/* 释放一个迭代器 */
void rb_free_iterator(rb_iterator iter);
/* 如果树为空，则返回1；否则返回0 */
bool rb_empty(rbtree_t *rb);
/* 返回树的大小 */
size_t rb_size(rbtree_t *rb);
/* 在树中查找指定节点，并返回其迭代器 */
rb_iterator rb_find(rbtree_t *rb, const void *key);
/* 返回key的名次 */
size_t rb_order_of_key(rbtree_t *rb, const void *key);
/* 返回第order名的元素 */
rb_iterator rb_find_by_order(rbtree_t *rb, size_t order);
/* 在树中插入一个节点 */
void rb_insert(rbtree_t *rb, void *key, void *value);
/* 从树中擦除一个节点 */
void rb_erase(rbtree_t *rb, const void *key);
/* 返回键值大于等于key的最小节点 */
rb_iterator rb_lower_bound(rbtree_t *rb, const void *key);
/* 返回键值小于等于key的最大节点 */
rb_iterator rb_upper_bound(rbtree_t *rb, const void *key);
/* 清空树 */
void rb_clear(rbtree_t *rb);
/* 释放一个树 */
void rb_free(rbtree_t *rb);

#endif // _LIBCSTL_RBTREE_H
