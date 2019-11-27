#include <string.h>
#include <stdbool.h>

#include "alloc.h"

#define DEQUE_BUF_SIZE 16 /* 每个节点存放的最大元素数 */

#define DEQUE_INCR_FACTOR 2

struct deque_node {
    unsigned char index; /* 下一个要插入的位置 */
    size_t map_index; /* 节点在map中的位置 */
    char buf[]; /* 数据缓冲区 */
};

typedef void (*__deque_free_handler)(void *);

typedef struct __deque {
    size_t size;
    size_t typesize;
    size_t front;
    size_t back;
    size_t pfs; /* push_front operation times */
    size_t pbs; /* push_back operation times */
    size_t mapsize;
    struct deque_node **map;
    __deque_free_handler deque_free;
    bool pointer;
} deque_t;

typedef struct __deque_iterator {
    deque_t *deque;
    struct deque_node *node;
    unsigned char index;  /* 元素在node->buf中的下标 */
} *deque_iterator;

typedef struct __deque_pos {
    size_t node;  /* 元素所在的节点 */
    size_t index;  /* 在该节点中的索引 */
} deque_pos;

#define __check_deque(d) (assert(d))

#define __front(d) (d->map[d->front])
#define __back(d) (d->map[d->back])

#define __front_off(x) (DEQUE_BUF_SIZE - 1 - x)

#define __get_pointer(x) (*(void**)(x))

static struct deque_node *__alloc_node(deque_t *d, size_t map_index)
{
    struct deque_node *node =
        Malloc(sizeof(struct deque_node) + d->typesize * DEQUE_BUF_SIZE);
    node->map_index = map_index;
    node->index = 0;
    return node;
}

static void __free_node(deque_t *d, struct deque_node *node, size_t start, size_t end)
{
    if (!d->deque_free) return;
    for (size_t i = start; i < end; i++) {
        void *ptr = *(void**)(node->buf + i * d->typesize);
        if (ptr) d->deque_free(ptr);
    }
}

static void __free_whole_node(deque_t *d, struct deque_node *node)
{
    __free_node(d, node, 0, DEQUE_BUF_SIZE);
}

static deque_iterator __alloc_iterator(deque_t *d, struct deque_node *node, size_t index)
{
    deque_iterator iter = Malloc(sizeof(*iter));
    iter->deque = d;
    iter->node = node;
    iter->index = index;
    return iter;
}

static void __deque_realloc(deque_t *d, size_t new_size)
{
    /* 0 <= ratio <= 1 */
    double ratio = (d->pfs * 1.0) / (d->pfs + d->pbs);
    size_t old_front = d->front;
    size_t old_size = d->back - d->front + 1;
    struct deque_node **old_map = d->map;
    d->front = (new_size - old_size) * ratio;
    d->back = d->front + old_size - 1;
    d->map = Calloc(new_size, sizeof(struct deque_node *));
    d->mapsize = new_size;
    memcpy(d->map + d->front, old_map + old_front, old_size * sizeof(struct deque_node *));
    size_t i = __front(d) ? d->front : d->front + 1;
    size_t j = __back(d) ? d->back : d->back - 1;
    for ( ; i <= j; i++)
        d->map[i]->map_index = i;
    free(old_map);
}

static char * __deque_off_ptr(deque_t *d, size_t map_index, size_t index)
{
    return d->map[map_index]->buf + index * d->typesize;
}

static struct deque_node *__deque_next_node(deque_iterator iter)
{
    return iter->deque->map[iter->node->map_index + 1];
}

static struct deque_node *__deque_prev_node(deque_iterator iter)
{
    return iter->deque->map[iter->node->map_index - 1];
}

static deque_pos __deque_get_pos(deque_t *d, size_t index)
{
    deque_pos pos;
    pos.node = __front(d) ? d->front : d->front + 1;
    pos.index = index;
    size_t size = __front(d) ? __front(d)->index : d->map[pos.node]->index;
    if (pos.index >= size) {
        size_t nodes = (pos.index - size) / DEQUE_BUF_SIZE;
        pos.index -= nodes * DEQUE_BUF_SIZE + size;
        pos.node += nodes + 1;
    } else {
        if (__front(d))
            pos.index += DEQUE_BUF_SIZE - size;
    }
    return pos;
}

static void __deque_free_after_index(deque_t *d, size_t index)
{
    if (!d->deque_free) return;
    deque_pos pos = __deque_get_pos(d, index);
    __free_node(d, d->map[pos.node], pos.index, DEQUE_BUF_SIZE);
    for (size_t i = pos.node + 1; i < d->back; i++)
        __free_whole_node(d, d->map[i]);
    __free_node(d, __back(d), 0, __back(d)->index);
}

deque_t *deque_init(size_t typesize)
{
    deque_t *d = Calloc(1, sizeof(deque_t));
    d->typesize = typesize;
    d->map = Calloc(2, sizeof(struct deque_node *));
    d->mapsize = 2;
    d->front = 0;
    d->back = 1;
    return d;
}

deque_t *deque_init_p(void)
{
    deque_t *d = deque_init(sizeof(void*));
    d->pointer = true;
    return d;
}

void deque_set_free_handler(deque_t *d, __deque_free_handler dfree)
{
    __check_deque(d);
    d->deque_free = dfree;
}

void *deque_entry(deque_t *d, size_t index)
{
    __check_deque(d);
    deque_pos pos = __deque_get_pos(d, index);
    return __deque_off_ptr(d, pos.node, pos.index);
}

void *deque_entry_p(deque_t *d, size_t index)
{
    return __get_pointer(deque_entry(d, index));
}

void *deque_front(deque_t *d)
{
    return deque_entry(d, 0);
}

void *deque_front_p(deque_t *d)
{
    return deque_entry_p(d, 0);
}

void *deque_back(deque_t *d)
{
    __check_deque(d);
    return d->size > 0 ? deque_entry(d, d->size - 1) : NULL;
}

void *deque_back_p(deque_t *d)
{
    return __get_pointer(deque_back(d));
}

deque_iterator deque_begin(deque_t *d)
{
    __check_deque(d);
    if (d->size == 0) return NULL;
    deque_pos pos = __deque_get_pos(d, 0);
    return __alloc_iterator(d, d->map[pos.node], pos.index);
}

deque_iterator deque_end(deque_t *d)
{
    __check_deque(d);
    if (d->size == 0) return NULL;
    deque_pos pos = __deque_get_pos(d, d->size - 1);
    return __alloc_iterator(d, d->map[pos.node], pos.index);
}

bool deque_next(deque_iterator iter)
{
    size_t limit = (iter->node->map_index == iter->deque->back)
        ? iter->node->index : DEQUE_BUF_SIZE;
    if (++iter->index == limit) {
        if (iter->node->map_index == iter->deque->back) return false;
        iter->node = __deque_next_node(iter);
        iter->index = 0;
    }
    return true;
}

bool deque_prev(deque_iterator iter)
{
    size_t limit = (iter->node->map_index == iter->deque->front)
        ? __front_off(iter->node->index + 1) : 0;
    if (iter->index == limit) {
        if (iter->node->map_index == iter->deque->front) return false;
        iter->node = __deque_prev_node(iter);
        iter->index = DEQUE_BUF_SIZE - 1;
    } else
        iter->index--;
    return true;
}

void *deque_get(deque_iterator iter)
{
    return iter->node->buf + iter->index * iter->deque->typesize;
}

void *deque_get_p(deque_iterator iter)
{
    return __get_pointer(deque_get(iter));
}

void deque_free_iterator(deque_iterator iter)
{
    free(iter);
}

bool deque_empty(deque_t *d)
{
    __check_deque(d);
    return d->size == 0;
}

size_t deque_size(deque_t *d)
{
    __check_deque(d);
    return d->size;
}

void deque_shrink_to_fit(deque_t *d)
{
    __check_deque(d);
    __deque_realloc(d, d->mapsize);
}

void deque_insert()
{
    /* TODO: */
}

void deque_erase()
{
    /* TODO: */
}

void deque_push_back(deque_t *d, const void *data)
{
    __check_deque(d);
    if (__back(d)) {
        if (__back(d)->index == DEQUE_BUF_SIZE) {
            if (d->back == d->mapsize - 1)
                __deque_realloc(d, d->mapsize * DEQUE_INCR_FACTOR);
            d->back++;
        }
    }
    if (!__back(d)) __back(d) = __alloc_node(d, d->back);
    char *ptr = __deque_off_ptr(d, d->back, __back(d)->index);
    if (d->pointer) memcpy(ptr, &data, d->typesize);
    else memcpy(ptr, data, d->typesize);
    __back(d)->index++;
    d->pbs++;
    d->size++;
}

void deque_pop_back(deque_t *d)
{
    __check_deque(d);
    if (d->size == 0) return;
    if (__back(d)) {
        if (--__back(d)->index == 0) {
            if (d->back - 1 == d->front) {
                __front(d)->index--;
            } else {
                __free_whole_node(d, __back(d));
                d->back--;
            }
        }
    } else {
        if (d->back - 1 == d->front) {
            __front(d)->index--;
        } else {
            __free_whole_node(d, __back(d));
            d->back--;
            __back(d)->index--;
        }
    }
    d->size--;
}

void deque_push_front(deque_t *d, const void *data)
{
    __check_deque(d);
    if (__front(d)) {
        if (__front(d)->index == DEQUE_BUF_SIZE) {
            if (d->front == 0)
                __deque_realloc(d, d->mapsize * DEQUE_INCR_FACTOR);
            d->front--;
        }
    }
    if (!__front(d)) __front(d) = __alloc_node(d, d->front);
    char *ptr = __deque_off_ptr(d, d->front, __front_off(__front(d)->index));
    if (d->pointer) memcpy(ptr, &data, d->typesize);
    else memcpy(ptr, data, d->typesize);
    __front(d)->index++;
    d->pfs++;
    d->size++;
}

void deque_pop_front(deque_t *d)
{
    __check_deque(d);
    if (d->size == 0) return;
    if (__front(d)) {
        if (--__front(d)->index == 0) {
            if (d->front + 1 == d->back) {
                __back(d)->index--;
            } else {
                __free_whole_node(d, __front(d));
                d->front++;
            }
        }
    } else {
        if (d->front + 1 == d->back) {
            __back(d)->index--;
        } else {
            __free_whole_node(d, __front(d));
            d->front++;
            __front(d)->index--;
        }
    }
    d->size--;
}

void deque_resize(deque_t *d, size_t count)
{
    __check_deque(d);
    size_t nodes = count / DEQUE_BUF_SIZE;
    size_t size = count % DEQUE_BUF_SIZE;
    if (nodes > d->mapsize || (nodes == d->mapsize && size > 0))
        __deque_realloc(d, nodes + 1);
    if (count > 0 && count < d->size) {
        __deque_free_after_index(d, count - 1);
    }
    d->size = count;
}

void deque_swap(deque_t *d, size_t i, size_t j)
{
    __check_deque(d);
    deque_pos pos1 = __deque_get_pos(d, i);
    deque_pos pos2 = __deque_get_pos(d, j);
    char *ptr1 = __deque_off_ptr(d, pos1.node, pos1.index);
    char *ptr2 = __deque_off_ptr(d, pos2.node, pos2.index);
    char buf[d->typesize];
    memcpy(buf, ptr1, d->typesize);
    memcpy(ptr1, ptr2, d->typesize);
    memcpy(ptr2, buf, d->typesize);
}

void deque_clear(deque_t *d)
{
    __check_deque(d);
    __deque_free_after_index(d, 0);
}

void deque_free(deque_t *d)
{
    deque_clear(d);
    for (size_t i = 0; i < d->mapsize; i++)
        if (d->map[i]) free(d->map[i]);
    free(d->map);
    free(d);
}
