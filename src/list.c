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

#define __front(list) (list->list->prev)
#define __back(list) (list->list->next)

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

void *list_get(struct list_node *node)
{
    return node->data;
}

struct list_node *list_front(list_t *list)
{
    __check_list(list);
    return __front(list);
}

struct list_node *list_back(list_t *list)
{
    __check_list(list);
    return __back(list);
}

struct list_node *list_next(struct list_node *node)
{
    return node->next;
}

struct list_node *list_prev(struct list_node *node)
{
    return node->prev;
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

struct list_node *list_find(list_t *list, const void *data)
{
    __check_list(list);
    struct list_node *iter = list_front(list);
    for ( ; iter; iter = iter->next) {
        if (list->list_comp(iter->data, data) == 0)
            return iter;
    }
    return NULL;
}

void list_insert(list_t *list, void *data)
{
    __check_list(list);
    struct list_node *node = __alloc_node(data);
    struct list_node *pos = list_find(list, data);
    if (pos) __list_insert_before(list, pos, node);
    else __list_insert_back(list, node);
}

void list_erase(list_t *list, const void *data)
{
    __check_list(list);
    struct list_node *node = list_find(list, data);
    if (node) __list_erase(list, node);
}

list_t *list_merge(list_t *list1, list_t *list2)
{
    __check_list(list1);
    __check_list(list2);
    list_t *list = list_init(list1->list_comp);
    struct list_node *iter1 = list_front(list1);
    struct list_node *iter2 = list_front(list2);
    struct list_node *_iter1 = iter1, *_iter2 = iter2;
    while (iter1 && iter2) {
        if (list->list_comp(iter1->data, iter2->data) == 0) {
            _iter1 = iter1->next;
            __list_insert_back(list, iter1);
        } else {
            _iter2 = iter2->next;
            __list_insert_back(list, iter2);
        }
        iter1 = _iter1;
        iter2 = _iter2;
    }
    while (iter1) {
        _iter1 = iter1->next;
        __list_insert_back(list, iter1);
        iter1 = _iter1;
    }
    while (iter2) {
        _iter2 = iter2->next;
        __list_insert_back(list, iter2);
        iter2 = _iter2;
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
    /* swap __front() and __back() */
    struct list_node *tmp = __front(list);
    __front(list) = __back(list);
    __back(list) = tmp;
}

void list_unique(list_t *list)
{
    __check_list(list);
    struct list_node *iter = list_front(list);
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
