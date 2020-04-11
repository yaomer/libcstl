#include <time.h>
#include <string.h>
#include <stdbool.h>

#include "alloc.h"

#define SKIPLIST_MAX_LEVEL 32 /* 1 ~ 32 */

#define SKIPLIST_P 0.25

struct sl_node {
    void *key;
    void *value;
    struct sl_node *prev;
    struct sl_node *next[];
};

typedef int (*__skiplist_comp_handler)(const void *, const void *);
typedef void (*__skiplist_free_handler)(void *, void *);

typedef struct __skiplist {
    int level;
    size_t size;
    struct sl_node *head;
    struct sl_node *begin;
    struct sl_node *end;
    __skiplist_comp_handler sl_comp;
    __skiplist_free_handler sl_free;
} skiplist_t;

typedef struct __sl_iterator {
    struct sl_node *node;
} *sl_iterator;

#define __check_skiplist(sl) (assert(sl))

static struct sl_node *__alloc_node(void *key, void *value, int level)
{
    struct sl_node *node =
        Calloc(1, sizeof(struct sl_node) + level * sizeof(node));
    node->key = key;
    node->value = value;
    return node;
}

static void __free_node(skiplist_t *sl, struct sl_node *node)
{
    if (sl->sl_free)
        sl->sl_free(node->key, node->value);
    free(node);
}

static sl_iterator __alloc_iterator(struct sl_node *node)
{
    sl_iterator iter = Malloc(sizeof(*iter));
    iter->node = node;
    return iter;
}

static int __sl_rand_level(void)
{
    int level = 1;
    while ((random() & 0xffff) < (SKIPLIST_P * 0xffff))
        level++;
    return (level < SKIPLIST_MAX_LEVEL) ? level : SKIPLIST_MAX_LEVEL;
}

/* 返回键大于等于key的最小节点 */
static struct sl_node *__sl_find(skiplist_t *sl, const void *key)
{
    struct sl_node *p = sl->head;
    struct sl_node *pre = p;

    for (int i = sl->level - 1; i >= 0; i--) {
        while ((p = pre->next[i]) && sl->sl_comp(key, p->key) > 0) {
            pre = p;
        }
        if (p && sl->sl_comp(key, p->key) == 0)
            break;
    }
    return p;
}

static void __sl_update_begin_and_end_in_insert(skiplist_t *sl, struct sl_node *node)
{
    if (!sl->begin) {
        sl->begin = sl->end = node;
        return;
    }
    if (sl->sl_comp(node->key, sl->begin->key) < 0)
        sl->begin = node;
    if (sl->sl_comp(node->key, sl->end->key) > 0)
        sl->end = node;
}

static void __sl_update_begin_and_end_in_erase(skiplist_t *sl, struct sl_node *node)
{
    if (node == sl->begin) sl->begin = node->next[0];
    if (node == sl->end) sl->end = node->prev;
}

static void __sl_insert(skiplist_t *sl, void *key, void *value)
{
    int level = __sl_rand_level();
    if (level > sl->level) {
        sl->level = level;
    }
    /* 寻找插入位置 */
    struct sl_node *update[sl->level];
    struct sl_node *pre = sl->head;
    struct sl_node *p = NULL;
    for (int i = sl->level - 1; i >= 0; i--) {
        while ((p = pre->next[i]) && sl->sl_comp(key, p->key) > 0) {
            pre = p;
        }
        update[i] = pre;
    }
    /* 已存在键为key的节点，更新它的值 */
    if (p && sl->sl_comp(key, p->key) == 0) {
        if (sl->sl_free) {
            sl->sl_free(NULL, p->value);
            sl->sl_free(key, NULL);
        }
        p->value = value;
        return;
    }
    p = __alloc_node(key, value, level);
    __sl_update_begin_and_end_in_insert(sl, p);
    /* 逐层插入 */
    for (int i = level - 1; i >= 0; i--) {
        p->next[i] = update[i]->next[i];
        update[i]->next[i] = p;
    }
    p->prev = (update[0] == sl->head) ? NULL : update[0];
    sl->size++;
}

static void __sl_erase(skiplist_t *sl, const void *key)
{
    struct sl_node *update[sl->level];
    struct sl_node *pre = sl->head;
    struct sl_node *p = NULL;

    for (int i = sl->level - 1; i >= 0; i--) {
        while ((p = pre->next[i]) && sl->sl_comp(key, p->key) > 0) {
            pre = p;
        }
        update[i] = pre;
    }
    if (!p || sl->sl_comp(key, p->key) != 0) return;
    for (int i = sl->level - 1; i >= 0; i--) {
        if (update[i]->next[i] == p) {
            update[i]->next[i] = p->next[i];
            if (!sl->head->next[i])
                sl->level--;
        }
    }
    __sl_update_begin_and_end_in_erase(sl, p);
    p->next[0]->prev = p->prev;
    __free_node(sl, p);
    sl->size--;
    return;
}

skiplist_t *sl_init(__skiplist_comp_handler slcomp, __skiplist_free_handler slfree)
{
    skiplist_t *sl = Calloc(1, sizeof(skiplist_t));
    sl->level = 1;
    sl->head = __alloc_node(NULL, NULL, SKIPLIST_MAX_LEVEL);
    sl->sl_comp = slcomp;
    sl->sl_free = slfree;
    srandom(time(NULL));
    return sl;
}

sl_iterator sl_begin(skiplist_t *sl)
{
    __check_skiplist(sl);
    return __alloc_iterator(sl->begin);
}

sl_iterator sl_end(skiplist_t *sl)
{
    __check_skiplist(sl);
    return __alloc_iterator(sl->end);
}

bool sl_next(sl_iterator iter)
{
    if (iter->node) iter->node = iter->node->next[0];
    return iter->node != NULL;
}

bool sl_prev(sl_iterator iter)
{
    if (iter->node) iter->node = iter->node->prev;
    return iter->node != NULL;
}

bool sl_equal(sl_iterator iter1, sl_iterator iter2)
{
    return iter1->node == iter2->node;
}

void *sl_get_key(sl_iterator iter)
{
    return iter->node->key;
}

void *sl_get_value(sl_iterator iter)
{
    return iter->node->value;
}

void sl_free_iterator(sl_iterator iter)
{
    free(iter);
}

sl_iterator sl_find(skiplist_t *sl, const void *key)
{
    __check_skiplist(sl);
    struct sl_node *p = __sl_find(sl, key);
    if (p && sl->sl_comp(key, p->key) == 0)
        return __alloc_iterator(p);
    return NULL;
}

void sl_insert(skiplist_t *sl, void *key, void *value)
{
    __check_skiplist(sl);
    __sl_insert(sl, key, value);
}

void sl_erase(skiplist_t *sl, const void *key)
{
    __check_skiplist(sl);
    __sl_erase(sl, key);
}

bool sl_empty(skiplist_t *sl)
{
    __check_skiplist(sl);
    return sl->size == 0;
}

size_t sl_size(skiplist_t *sl)
{
    __check_skiplist(sl);
    return sl->size;
}

sl_iterator sl_lower_bound(skiplist_t *sl, const void *key)
{
    struct sl_node *p = __sl_find(sl, key);
    return p ? __alloc_iterator(p) : NULL;
}

sl_iterator sl_upper_bound(skiplist_t *sl, const void *key)
{
    struct sl_node *p = __sl_find(sl, key);
    p = p ? (sl->sl_comp(key, p->key) == 0 ? p : p->prev) : sl->end;
    return __alloc_iterator(p);
}

void sl_clear(skiplist_t *sl)
{
    __check_skiplist(sl);
    /* 只需要遍历第一层即可 */
    struct sl_node *p = sl->head->next[0];
    while (p) {
        struct sl_node *np = p->next[0];
        __free_node(sl, p);
        p = np;
    }
    sl->level = 1;
    sl->size = 0;
    memset(sl->head->next, 0, SKIPLIST_MAX_LEVEL * sizeof(struct sl_node *));
}

void sl_free(skiplist_t *sl)
{
    sl_clear(sl);
    free(sl);
}
