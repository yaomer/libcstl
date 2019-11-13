#ifndef _LIBCSTL_HASH_H
#define _LIBCSTL_HASH_H

#include <stddef.h> /* for size_t */

typedef struct __hash hash_t;
typedef struct __hash_iterator * hash_iterator;

/* hash函数 */
typedef size_t (*__hash_handler)(const void *);
/* 查找和删除时用于判断键值是否相等 */
typedef int (*__hash_comp_handler)(const void *, const void *);
/* 用于释放存储的键 */
typedef void (*__hash_free_key_handler)(void *);
/* 用于释放存储的值 */
typedef void (*__hash_free_data_handler)(void *);

void hash_set_free_handler(hash_t *hash,
                           __hash_free_key_handler freekey,
                           __hash_free_data_handler freedata);
/* 初始化一个hash */
hash_t *hash_init(__hash_handler hash, __hash_comp_handler comp);
/* 返回指向hash首元素的迭代器 */
hash_iterator hash_begin(hash_t *hash);
/* 将iter修改为指向下一个元素的迭代器，如果成功则返回1，否则返回0 */
int hash_next(hash_iterator iter);
/* 取出迭代器中的键 */
void *hash_get_key(hash_iterator iter);
/* 取出迭代器中的值 */
void *hash_get_data(hash_iterator iter);
/* 释放一个迭代器 */
void hash_free_iterator(hash_iterator iter);
/* 在hash中查找一个指定元素 */
hash_iterator hash_find(hash_t *hash, const void *key);
/* 在hash中插入一个键值对 */
void hash_insert(hash_t *hash, void *key, void *data);
/* 从hash中擦除一个键值对 */
void hash_erase(hash_t *hash, const void *key);
/* 返回hash中的桶数 */
size_t hash_bucket_count(hash_t *hash);
/* 返回特定桶中的元素数量 */
size_t hash_bucket_size(hash_t *hash, size_t n);
/* 返回存储特定键的桶 */
size_t hash_bucket(hash_t *hash, const void *key);
/* 清空hash中的所有元素 */
void hash_clear(hash_t *hash);
/* 释放一个hash */
void hash_free(hash_t *hash);

/*
 * 遍历一个hash：
 * iter = hash_begin(hash);
 * do { do_something(hash_get(iter)); } while (hash_next(iter));
 */

#endif /* _LIBCSTL_HASH_H */
