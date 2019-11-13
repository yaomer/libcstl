#include "alloc.h"

struct list_node {
    void *data;
    struct list_node *prev;
    struct list_node *next;
};

typedef int (*__list_comp_handler)(const void *, const void *);
typedef void (*__list_free_data_handler)(void *);

typedef struct __list {
    size_t size;
    /* list->prev -> list head
     * list->next -> list tail */
    struct list_node *list;
    __list_comp_handler list_comp;
    __list_free_data_handler list_free_data;
} list_t;

struct __list_iterator {
    struct list_node *node;
};

typedef struct __list_iterator * list_iterator;

#define __front(_list) ((_list)->list->prev)
#define __back(_list) ((_list)->list->next)

#define __check_list(list) (assert(list))

static void __list_free_node(list_t *list, struct list_node *node)
{
    if (list->list_free_data)
        list->list_free_data(node->data);
    free(node);
    list->size--;
}

static struct list_node *__alloc_node(void *data)
{
    struct list_node *node = Calloc(1, sizeof(struct list_node));
    node->data = data;
    return node;
}

static list_iterator __alloc_iterator(struct list_node *node)
{
    list_iterator iter = Malloc(sizeof(*iter));
    iter->node = node;
    return iter;
}

static struct list_node *__list_find(list_t *list, const void *data)
{
    __check_list(list);
    struct list_node *iter = __front(list);
    for ( ; iter; iter = iter->next) {
        if (list->list_comp(iter->data, data) == 0)
            return iter;
    }
    return NULL;
}

static void __list_insert_front(list_t *list, struct list_node *node)
{
    if (__front(list)) {
        node->next = __front(list);
        __front(list)->prev = node;
    } else {
        __back(list) = node;
    }
    __front(list) = node;
    list->size++;
}

static void __list_insert_back(list_t *list, struct list_node *node)
{
    if (__back(list)) {
        __back(list)->next = node;
        node->prev = __back(list);
    } else {
        __front(list) = node;
    }
    __back(list) = node;
    list->size++;
}

static void __list_insert_before(list_t *list,
        struct list_node *pos, struct list_node *node)
{
    node->prev = pos->prev;
    if (pos->prev)
        pos->prev->next = node;
    pos->prev = node;
    node->next = pos;
    if (!node->prev)
        __front(list) = node;
    list->size++;
}

static void __list_insert(list_t *list, struct list_node *node, int reverse)
{
    struct list_node *iter = __front(list);
    for ( ; iter; iter = iter->next) {
        if (reverse) {
            if (list->list_comp(node->data, iter->data) > 0) {
                __list_insert_before(list, iter, node);
                return;
            }
        } else {
            if (list->list_comp(node->data, iter->data) < 0) {
                __list_insert_before(list, iter, node);
                return;
            }
        }
    }
    __list_insert_back(list, node);
}

static void __list_pop_front(list_t *list)
{
    if (__front(list)) {
        struct list_node *node = __front(list);
        __front(list) = __front(list)->next;
        if (__front(list))
            __front(list)->prev = NULL;
        else
            __back(list) = NULL;
        __list_free_node(list, node);
    }
}

static void __list_pop_back(list_t *list)
{
    if (__back(list)) {
        struct list_node *node = __back(list);
        __back(list) = __back(list)->prev;
        if (__back(list))
            __back(list)->next = NULL;
        else
            __front(list) = NULL;
        __list_free_node(list, node);
    }
}

static void __list_erase(list_t *list, struct list_node *node)
{
    if (node->prev && node->next) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    } else if (node->prev) {
        node->prev->next = NULL;
        __back(list) = node->prev;
    } else if (node->next) {
        node->next->prev = NULL;
        __front(list) = node->next;
    } else {
        __front(list) = __back(list) = NULL;
    }
    __list_free_node(list, node);
}

list_t *list_init(__list_comp_handler comp)
{
    list_t *list = Calloc(1, sizeof(list_t));
    list->list = Calloc(1, sizeof(struct list_node));
    list->list_comp = comp;
    return list;
}

void list_set_free_data_handler(list_t *list, void (*list_free_data)(void *))
{
    list->list_free_data = list_free_data;
}

int list_empty(list_t *list)
{
    return list->size == 0;
}

size_t list_size(list_t *list)
{
    return list->size;
}

void *list_get(list_iterator iter)
{
    return iter->node->data;
}

list_iterator list_front(list_t *list)
{
    __check_list(list);
    return __alloc_iterator(__front(list));
}

list_iterator list_back(list_t *list)
{
    __check_list(list);
    return __alloc_iterator(__back(list));
}

int list_next(list_iterator iter)
{
    if (iter->node) iter->node = iter->node->next;
    return iter->node != NULL;
}

int list_prev(list_iterator iter)
{
    if (iter->node) iter->node = iter->node->prev;
    return iter->node != NULL;
}

void list_free_iterator(list_iterator iter)
{
    free(iter);
}

void list_push_front(list_t *list, void *data)
{
    __check_list(list);
    __list_insert_front(list, __alloc_node(data));
}

void list_pop_front(list_t *list)
{
    __check_list(list);
    __list_pop_front(list);
}

void list_push_back(list_t *list, void *data)
{
    __check_list(list);
    __list_insert_back(list, __alloc_node(data));
}

void list_pop_back(list_t *list)
{
    __check_list(list);
    __list_pop_back(list);
}

void list_insert_before(list_t *list, struct list_node *pos, void *data)
{
    __check_list(list);
    if (pos) __list_insert_before(list, pos, __alloc_node(data));
}

list_iterator list_find(list_t *list, const void *data)
{
    struct list_node *node = __list_find(list, data);
    return node ? __alloc_iterator(node) : NULL;
}

void list_insert(list_t *list, void *data)
{
    __check_list(list);
    __list_insert(list, __alloc_node(data), 0);
}

void list_insert_reverse(list_t *list, void *data)
{
    __check_list(list);
    __list_insert(list, __alloc_node(data), 1);
}

void list_erase(list_t *list, const void *data)
{
    __check_list(list);
    struct list_node *node = __list_find(list, data);
    if (node) __list_erase(list, node);
}

list_t *list_merge(list_t *list1, list_t *list2)
{
    __check_list(list1);
    __check_list(list2);
    list_t *list = list_init(list1->list_comp);
    struct list_node *iter1 = __front(list1);
    struct list_node *iter2 = __front(list2);
    struct list_node *_iter1 = iter1, *_iter2 = iter2;
    while (iter1 && iter2) {
        if (list->list_comp(iter1->data, iter2->data) < 0) {
            _iter1 = iter1->next;
            __list_insert_back(list, iter1);
            list1->size--;
        } else {
            _iter2 = iter2->next;
            __list_insert_back(list, iter2);
            list2->size--;
        }
        iter1 = _iter1;
        iter2 = _iter2;
    }
    if (iter1) {
        __back(list)->next = iter1;
        __back(list) = __back(list1);
        list->size += list1->size;
    } else if (iter2) {
        __back(list)->next = iter2;
        __back(list) = __back(list2);
        list->size += list2->size;
    }
    free(list1->list);
    free(list1);
    free(list2->list);
    free(list2);
    return list;
}

void list_reverse(list_t *list)
{
    __check_list(list);
    struct list_node *iter = __front(list);
    struct list_node *_iter;
    __front(list) = __back(list) = NULL;
    while (iter) {
        _iter = iter->next;
        iter->next = iter->prev = NULL;
        __list_insert_front(list, iter);
        iter = _iter;
    }
}

void list_unique(list_t *list)
{
    __check_list(list);
    struct list_node *iter = __front(list);
    while (iter && iter->next) {
        if (list->list_comp(iter->data, iter->next->data) == 0) {
            __list_erase(list, iter->next);
        } else
            iter = iter->next;
    }
}

void list_clear(list_t *list)
{
    __check_list(list);
    while (__front(list))
        list_pop_front(list);
}

void list_free(list_t *list)
{
    list_clear(list);
    free(list->list);
    free(list);
}
