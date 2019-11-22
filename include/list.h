#ifndef _LIBCSTL_LIST_H
#define _LIBCSTL_LIST_H

#include <stddef.h> /* for size_t */
#include <stdbool.h>

typedef struct __list list_t;
typedef struct __list_iterator * list_iterator;
/* 用于比较链表节点 */
typedef int (*__list_comp_handler)(const void *, const void *);
/* 用于释放链表节点中存储的数据，如果不提供，则在释放节点时不释放数据
 * 它通常有两种选择，一种是free()，一种是自定义的释放函数 */
typedef void (*__list_free_data_handler)(void *);

/* 初始化一个list */
list_t *list_init(__list_comp_handler comp);
/* 设置list释放数据时调用的函数 */
void list_set_free_data_handler(__list_free_data_handler);
/* 释放一个list */
void list_free(list_t *list);

/* 访问list的第一个元素 */
list_iterator list_front(list_t *list);
/* 访问list的最后一个元素 */
list_iterator list_back(list_t *list);
/* 将iter修改为指向下一个元素的迭代器
 * 如果iter为空或者修改后为空，则返回0，否则返回1 */
int list_next(list_iterator iter);
/* 将iter修改为指向上一个元素的迭代器
 * 如果iter为空或者修改后为空，则返回0，否则返回1 */
int list_prev(list_iterator iter);
/* 释放一个迭代器 */
void list_free_iterator(list_iterator iter);
/* 取出迭代器中的数据 */
void *list_get(list_iterator iter);

/* list是否为空 */
bool list_empty(list_t *list);
/* 返回list的容量 */
size_t list_size(list_t *list);

/* 清空list中的所有元素，此时list的状态同list_init() */
void list_clear(list_t *list);
/* 在list末尾插入一个元素 */
void list_push_back(list_t *list, void *data);
/* 移除末元素 */
void list_pop_back(list_t *list);
/* 在list首部插入一个元素 */
void list_push_front(list_t *list, void *data);
/* 移除首元素 */
void list_pop_front(list_t *list);
/* 按升序顺序在list中插入一个元素 */
void list_insert(list_t *list, void *data);
/* 按降序顺序在list中插入一个元素 */
void list_insert_reverse(list_t *list, void *data);
/* 在pos之前插入一个元素 */
void list_insert_before(list_t *list, list_iterator pos, void *data);
/* 从list中移除一个元素 */
void list_erase(list_t *list, const void *data);

/* 在list中查找一个指定元素 */
list_iterator list_find(list_t *list, const void *data);
/* 合并两个按升序排列的链表
 * 合并之后，list1和list2将会被释放，并返回一个新的list */
list_t *list_merge(list_t *list1, list_t *list2);
/* 逆置list */
void list_reverse(list_t *list);
/* 移除list中连续的重复元素，只保留第一个 */
void list_unique(list_t *list);

#endif // _LIBCSTL_LIST_H
