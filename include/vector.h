#ifndef _LIBCSTL_VECTOR_H
#define _LIBCSTL_VECTOR_H

#include <stddef.h> /* for size_t */

typedef struct __vector vector_t;
typedef struct __vector_iterator * vector_iterator;
/* vector拷贝数据时默认执行的操作是：memcpy(des, user_data, typesize)
 * __vector_copy_handler可用于让用户定义自己需要的拷贝行为 */
typedef void (*__vector_copy_handler)(void *, const void *);
/* 用于释放存储的数据 */
typedef void (*__vector_free_handler)(void *);

/* 初始化一个vector */
vector_t *vector_init(size_t typesize);
/* 设置拷贝函数 */
void vector_set_copy_handler(vector_t *v, __vector_copy_handler vcopy);
/* 设置释放函数 */
void vector_set_free_handler(vector_t *v, __vector_free_handler vfree);
/* return vector[index]，不进行越界检查 */
void *vector_entry(vector_t *vector, size_t index);
/* 返回vector首元素的迭代器 */
vector_iterator vector_begin(vector_t *v);
/* 返回指向下一个节点的迭代器，如果iter为空或者下一个节点为空，则返回0；否则返回1 */
int vector_next(vector_iterator iter);
/* 取出迭代器中的数据 */
void *vector_get(vector_iterator iter);
/* 释放一个迭代器 */
void vector_free_iterator(vector_iterator iter);
/* 判断vector是否为空 */
int vector_empty(vector_t *v);
/* 返回容纳的元素数 */
size_t vector_size(vector_t *v);
/* 返回当前存储空间能够容纳的元素数 */
size_t vector_capacity(vector_t *v);
/* 预留存储空间 */
void vector_reserve(vector_t *v, size_t new_cap);
/* 改变vector中容纳元素的个数 */
void vector_resize(vector_t *v, size_t count);
/* 释放未使用的内存 */
void vector_shrink_to_fit(vector_t *v);
/* 在pos之前插入一个元素 */
void vector_insert(vector_t *v, vector_iterator pos, const void *data);
/* 删除pos处的元素 */
void vector_erase(vector_t *v, vector_iterator pos);
/* 在vector末尾添加一个元素 */
void vector_push_back(vector_t *vector, const void *data);
/* 从vector末尾弹出一个元素 */
void vector_pop_back(vector_t *v);
/* 交换两个元素 */
void vector_swap(vector_t *v, size_t i, size_t j);
/* 清空vector */
void vector_clear(vector_t *v);
/* 释放vector */
void vector_free(vector_t *v);

#endif // _LIBCSTL_VECTOR_H