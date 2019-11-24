#include <time.h>
#include <string.h>
#include <stdbool.h>

#include "alloc.h"

#define SKIPLIST_MAX_LEVEL 32 /* 1 ~ 32 */

#define SKIPLIST_P 0.25

struct skiplist_node {
    void *key;
    void *value;
    struct skiplist_node *prev;
    struct skiplist_node *next[];
};

typedef int (*__skiplist_comp_handler)(const void *, const void *);
typedef void (*__skiplist_free_handler)(void *, void *);

typedef struct __skiplist {
    int level;
    size_t size;
    struct skiplist_node *head;
    struct skiplist_node *begin;
    struct skiplist_node *end;
    __skiplist_comp_handler sl_comp;
    __skiplist_free_handler sl_free;
} skiplist_t;

typedef struct __skiplist_iterator {
    struct skiplist_node *node;
} *skiplist_iterator;

#define __check_skiplist(sl) (assert(sl))

static struct skiplist_node *__alloc_node(void *key, void *value, int level)
{
    struct skiplist_node *node =
        Calloc(1, sizeof(struct skiplist_node) + level * sizeof(node));
    node->key = key;
    node->value = value;
    return node;
}

static void __free_node(skiplist_t *sl, struct skiplist_node *node)
{
    if (sl->sl_free)
        sl->sl_free(node->key, node->value);
    free(node);
}

static skiplist_iterator __alloc_iterator(struct skiplist_node *node)
{
    skiplist_iterator iter = Malloc(sizeof(*iter));
    iter->node = node;
    return iter;
}

static int __skiplist_rand_level(void)
{
    int level = 1;
    while ((random() & 0xffff) < (SKIPLIST_P * 0xffff))
        level++;
    return (level < SKIPLIST_MAX_LEVEL) ? level : SKIPLIST_MAX_LEVEL;
}

/* 返回键大于等于key的最小节点 */
static struct skiplist_node *__skiplist_find(skiplist_t *sl, const void *key)
{
    struct skiplist_node *p = sl->head;
    struct skiplist_node *pre = p;

    for (int i = sl->level - 1; i >= 0; i--) {
        while ((p = pre->next[i]) && sl->sl_comp(key, p->key) > 0) {
            pre = p;
        }
        if (p && sl->sl_comp(key, p->key) == 0)
            break;
    }
    return p;
}

static void __skiplist_update_begin_and_end_in_insert(skiplist_t *sl, struct skiplist_node *node)
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

static void __skiplist_update_begin_and_end_in_erase(skiplist_t *sl, struct skiplist_node *node)
{
    if (node == sl->begin) sl->begin = node->next[0];
    if (node == sl->end) sl->end = node->prev;
}

static void __skiplist_insert(skiplist_t *sl, void *key, void *value)
{
    int level = __skiplist_rand_level();
    if (level > sl->level) {
        sl->level = level;
    }
    /* 寻找插入位置 */
    struct skiplist_node *update[sl->level];
    struct skiplist_node *pre = sl->head;
    struct skiplist_node *p = NULL;
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
    __skiplist_update_begin_and_end_in_insert(sl, p);
    /* 逐层插入 */
    for (int i = level - 1; i >= 0; i--) {
        p->next[i] = update[i]->next[i];
        update[i]->next[i] = p;
    }
    p->prev = (update[0] == sl->head) ? NULL : update[0];
    sl->size++;
}

static void __skiplist_erase(skiplist_t *sl, const void *key)
{
    struct skiplist_node *update[sl->level];
    struct skiplist_node *pre = sl->head;
    struct skiplist_node *p = NULL;

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
    __skiplist_update_begin_and_end_in_erase(sl, p);
    p->next[0]->prev = p->prev;
    __free_node(sl, p);
    sl->size--;
    return;
}

skiplist_t *skiplist_init(__skiplist_comp_handler slcomp, __skiplist_free_handler slfree)
{
    skiplist_t *sl = Calloc(1, sizeof(skiplist_t));
    sl->level = 1;
    sl->head = __alloc_node(NULL, NULL, SKIPLIST_MAX_LEVEL);
    sl->sl_comp = slcomp;
    sl->sl_free = slfree;
    srandom(time(NULL));
    return sl;
}

skiplist_iterator skiplist_begin(skiplist_t *sl)
{
    __check_skiplist(sl);
    return __alloc_iterator(sl->begin);
}

skiplist_iterator skiplist_end(skiplist_t *sl)
{
    __check_skiplist(sl);
    return __alloc_iterator(sl->end);
}

bool skiplist_next(skiplist_iterator iter)
{
    if (iter->node) iter->node = iter->node->next[0];
    return iter->node != NULL;
}

bool skiplist_prev(skiplist_iterator iter)
{
    if (iter->node) iter->node = iter->node->prev;
    return iter->node != NULL;
}

bool skiplist_equal(skiplist_iterator iter1, skiplist_iterator iter2)
{
    return iter1->node == iter2->node;
}

void *skiplist_get_key(skiplist_iterator iter)
{
    return iter->node->key;
}

void *skiplist_get_value(skiplist_iterator iter)
{
    return iter->node->value;
}

void skiplist_free_iterator(skiplist_iterator iter)
{
    free(iter);
}

skiplist_iterator skiplist_find(skiplist_t *sl, const void *key)
{
    __check_skiplist(sl);
    struct skiplist_node *p = __skiplist_find(sl, key);
    if (p && sl->sl_comp(key, p->key) == 0)
        return __alloc_iterator(p);
    return NULL;
}

void skiplist_insert(skiplist_t *sl, void *key, void *value)
{
    __check_skiplist(sl);
    __skiplist_insert(sl, key, value);
}

void skiplist_erase(skiplist_t *sl, const void *key)
{
    __check_skiplist(sl);
    __skiplist_erase(sl, key);
}

bool skiplist_empty(skiplist_t *sl)
{
    __check_skiplist(sl);
    return sl->size == 0;
}

size_t skiplist_size(skiplist_t *sl)
{
    __check_skiplist(sl);
    return sl->size;
}

skiplist_iterator skiplist_lower_bound(skiplist_t *sl, const void *key)
{
    struct skiplist_node *p = __skiplist_find(sl, key);
    return p ? __alloc_iterator(p) : NULL;
}

skiplist_iterator skiplist_upper_bound(skiplist_t *sl, const void *key)
{
    struct skiplist_node *p = __skiplist_find(sl, key);
    p = p ? (sl->sl_comp(key, p->key) == 0 ? p : p->prev) : sl->end;
    return __alloc_iterator(p);
}

void skiplist_clear(skiplist_t *sl)
{
    __check_skiplist(sl);
    /* 只需要遍历第一层即可 */
    struct skiplist_node *p = sl->head->next[0];
    while (p) {
        struct skiplist_node *np = p->next[0];
        __free_node(sl, p);
        p = np;
    }
    sl->level = 1;
    sl->size = 0;
    memset(sl->head->next, 0, SKIPLIST_MAX_LEVEL * sizeof(struct skiplist_node *));
}

void skiplist_free(skiplist_t *sl)
{
    skiplist_clear(sl);
    free(sl);
}
