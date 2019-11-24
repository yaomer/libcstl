#include <string.h>
#include <stdbool.h>

#include "alloc.h"

struct hash_node {
    void *key;
    void *value;
    struct hash_node *next;
};

typedef size_t (*__hash_handler)(const void *);
typedef bool (*__hash_equal_handler)(const void *, const void *);
typedef void (*__hash_free_handler)(void *, void *);

typedef struct __hash {
    size_t hashsize;  /* 哈希表的大小，即桶的数目 */
    size_t hashnums;  /* 当前已插入到hash中的元素数量 */
    size_t count; /* 如果发生扩容或缩容操作，则count计数器加1 */
    struct hash_node **buckets;
    __hash_handler hash;
    __hash_equal_handler hash_equal;
    __hash_free_handler hash_free;
} hash_t;

struct __hash_iterator {
    hash_t *hash; /* 指向所属的哈希表 */
    /* 如果在返回迭代器后，hash出现了扩容或缩容(count != hash->count)
     * 需要重新定位key，不过这种情况并不常见 */
    size_t count; /* 指向具体的存储节点 */
    struct hash_node *node;
};

typedef struct __hash_iterator * hash_iterator;

/* hash表的初始大小 */
#define HASH_INIT_SIZE 16
/* 扩容阀值 */
#define HASH_MAX_LOAD_FACTOR 0.75
/* 缩容阀值 */
#define HASH_MIN_LOAD_FACTOR 0.25
/* 扩容或缩容时的变化因子 */
#define HASH_INCR_FACTOR 2

#define __check_hash(hash) (assert(hash))

static struct hash_node **__alloc_buckets(size_t size)
{
    struct hash_node **buckets = Calloc(size, sizeof(struct hash_node *));
    return buckets;
}

static struct hash_node *__alloc_node(void *key, void *value)
{
    struct hash_node *node = Calloc(1, sizeof(struct hash_node));
    node->key = key;
    node->value = value;
    return node;
}

static hash_iterator __alloc_iterator(void)
{
    hash_iterator iter = Malloc(sizeof(*iter));
    return iter;
}

static size_t __hash(hash_t *hash, const void *key)
{
    return hash->hash(key) & (hash->hashsize - 1);
}

struct hash_node *__hash_find(hash_t *hash, const void *key)
{
    __check_hash(hash);
    size_t hashval = __hash(hash, key);
    struct hash_node *node = hash->buckets[hashval];
    for ( ; node; node = node->next)
        if (hash->hash_equal(node->key, key))
            return node;
    return NULL;
}

static void __hash_insert(hash_t *hash, struct hash_node *node)
{
    size_t hashval = __hash(hash, node->key);
    node->next = hash->buckets[hashval];
    hash->buckets[hashval] = node;
    hash->hashnums++;
}

static double __hash_load_factor(hash_t *hash)
{
    return (hash->hashnums * 1.0) / hash->hashsize;
}

static void __hash_expand_or_shrink(hash_t *hash, int expand)
{
    struct hash_node **old_buckets = hash->buckets;
    size_t old_hashsize = hash->hashsize;
    if (expand)
        hash->hashsize *= HASH_INCR_FACTOR;
    else {
        hash->hashsize /= HASH_INCR_FACTOR;
    }
    hash->buckets = __alloc_buckets(hash->hashsize);
    hash->hashnums = 0;

    for (size_t i = 0; i < old_hashsize; i++) {
        struct hash_node *node = old_buckets[i];
        while (node) {
            struct hash_node *tmp = node->next;
            __hash_insert(hash, node);
            node = tmp;
        }
    }
    free(old_buckets);
    hash->count++;
}

static void __hash_expand(hash_t *hash)
{
    if (__hash_load_factor(hash) >= HASH_MAX_LOAD_FACTOR)
        __hash_expand_or_shrink(hash, 1);
}

static void __hash_shrink(hash_t *hash)
{
    if (__hash_load_factor(hash) <= HASH_MIN_LOAD_FACTOR)
        __hash_expand_or_shrink(hash, 0);
}

static void __hash_free_node(hash_t *hash, struct hash_node *node)
{
    if (hash->hash_free)
        hash->hash_free(node->key, node->value);
    free(node);
    hash->hashnums--;
}

static void __hash_set_iterator(hash_iterator iter, hash_t *hash,
        struct hash_node *node)
{
    iter->hash = hash;
    iter->node = node;
    iter->count = hash->count;
}

hash_t *hash_init(__hash_handler hhash, __hash_equal_handler hequal, __hash_free_handler hfree)
{
    hash_t *hash = Calloc(1, sizeof(hash_t));
    hash->hashsize = HASH_INIT_SIZE;
    hash->buckets = __alloc_buckets(hash->hashsize);
    hash->hash = hhash;
    hash->hash_equal = hequal;
    hash->hash_free = hfree;
    return hash;
}

hash_iterator hash_begin(hash_t *hash)
{
    __check_hash(hash);
    hash_iterator iter = __alloc_iterator();
    for (size_t i = 0; i < hash->hashsize; i++) {
        if (hash->buckets[i]) {
            __hash_set_iterator(iter, hash, hash->buckets[i]);
            return iter;
        }
    }
    free(iter);
    return NULL;
}

hash_iterator hash_end(hash_t *hash)
{
    __check_hash(hash);
    hash_iterator iter = __alloc_iterator();
    for (ssize_t i = hash->hashsize - 1; i >= 0; i--) {
        if (hash->buckets[i]) {
            __hash_set_iterator(iter, hash, hash->buckets[i]);
            return iter;
        }
    }
    free(iter);
    return NULL;
}

bool hash_next(hash_iterator iter)
{
    hash_t *hash = iter->hash;
    if (iter->count != hash->count) {
        /* 重新定位iter->node */
        iter->node = __hash_find(hash, iter->node->key);
        /* iter已被删除 */
        if (!iter->node) return 0;
    }
    if (iter->node->next) {
        __hash_set_iterator(iter, hash, iter->node->next);
        return 1;
    }
    /* 查找相邻的下一个桶 */
    size_t hashval = __hash(hash, iter->node->key);
    for (size_t i = hashval + 1; i < hash->hashsize; i++) {
        if (hash->buckets[i]) {
            __hash_set_iterator(iter, hash, hash->buckets[i]);
            return 1;
        }
    }
    return 0;
}

void *hash_get_key(hash_iterator iter)
{
    return iter->node->key;
}

void *hash_get_value(hash_iterator iter)
{
    return iter->node->value;
}

void hash_free_iterator(hash_iterator iter)
{
    free(iter);
}

bool hash_empty(hash_t *hash)
{
    __check_hash(hash);
    return hash->hashsize == 0;
}

size_t hash_size(hash_t *hash)
{
    __check_hash(hash);
    return hash->hashsize;
}

hash_iterator hash_find(hash_t *hash, const void *key)
{
    struct hash_node *node = __hash_find(hash, key);
    if (!node) return NULL;
    hash_iterator iter = __alloc_iterator();
    __hash_set_iterator(iter, hash, node);
    return iter;
}

void hash_insert(hash_t *hash, void *key, void *value)
{
    __check_hash(hash);
    __hash_insert(hash, __alloc_node(key, value));
    __hash_expand(hash);
}

void hash_erase(hash_t *hash, const void *key)
{
    __check_hash(hash);
    size_t hashval = __hash(hash, key);
    struct hash_node *node = hash->buckets[hashval];
    struct hash_node *pre = NULL;

    for ( ; node; node = node->next) {
        if (hash->hash_equal(node->key, key))
            break;
        pre = node;
    }
    if (!node) return;
    if (pre)
        pre->next = node->next;
    else
        hash->buckets[hashval] = node->next;
    __hash_free_node(hash, node);
    __hash_shrink(hash);
}

size_t hash_bucket_count(hash_t *hash)
{
    __check_hash(hash);
    return hash->hashsize;
}

size_t hash_bucket_size(hash_t *hash, size_t n)
{
    __check_hash(hash);
    size_t size = 0;
    struct hash_node *node = hash->buckets[n];
    while (node) {
        node = node->next;
        size++;
    }
    return size;
}

size_t hash_bucket(hash_t *hash, const void *key)
{
    __check_hash(hash);
    return __hash(hash, key);
}

void hash_clear(hash_t *hash)
{
    __check_hash(hash);
    for (size_t i = 0; i < hash->hashsize; i++) {
        struct hash_node *node = hash->buckets[i];
        while (node) {
            struct hash_node *tmp = node->next;
            __hash_free_node(hash, node);
            node = tmp;
        }
        hash->buckets[i] = NULL;
    }
}

void hash_free(hash_t *hash)
{
    hash_clear(hash);
    free(hash->buckets);
    free(hash);
}
