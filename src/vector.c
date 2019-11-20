#include <string.h>

#include "alloc.h"

typedef void (*__vector_copy_handler)(void *, const void *);
typedef void (*__vector_free_handler)(void *);

typedef struct __vector {
    void *data;     /* 一个可变数组 */
    size_t size;    /* 已使用大小 */
    size_t alloc_size;  /* 已分配的内存大小 */
    size_t typesize;  /* 存储的类型大小 */
    __vector_copy_handler vector_copy;
    __vector_free_handler vector_free;
} vector_t;

typedef struct __vector_iterator {
    size_t index;
    vector_t *vector;
} *vector_iterator;

#define VECTOR_INIT_SIZE 16
#define VECTOR_INCR_FACTOR 2

#define __check_vector(v) (assert(v))

static size_t __vector_offset(vector_t *v, size_t n)
{
    return n * v->typesize;
}

static void __vector_realloc(vector_t *v, size_t new_count)
{
    v->data = Realloc(v->data, __vector_offset(v, new_count));
    v->alloc_size = new_count;
}

static void __vector_free(vector_t *v, size_t start)
{
    if (!v->vector_free) return;
    for (size_t i = start; i < v->size; i++)
        v->vector_free(v->data + __vector_offset(v, i));
}

static vector_iterator __alloc_iterator(vector_t *v, size_t index)
{
    vector_iterator iter = Malloc(sizeof(*iter));
    iter->index = index;
    iter->vector = v;
    return iter;
}

/* 在pos处插入一个元素 */
static void __vector_insert(vector_t *v, size_t pos, const void *data)
{
    if (v->size + 1 > v->alloc_size)
        __vector_realloc(v, v->alloc_size * VECTOR_INCR_FACTOR);
    void *p = v->data + __vector_offset(v, pos);
    if (pos < v->size) {
        memmove(p + v->typesize, p, __vector_offset(v, v->size - pos));
    }
    if (v->vector_copy)
        v->vector_copy(p, data);
    else
        memcpy(p, data, v->typesize);
    v->size++;
}

static void __vector_erase(vector_t *v, size_t pos)
{
    if (pos >= v->size) return;
    void *p = v->data + __vector_offset(v, pos);
    if (pos < v->size - 1) {
        memmove(p, p + v->typesize, __vector_offset(v, v->size - 1 - pos));
    }
    v->size--;
}

vector_t *vector_init(size_t typesize)
{
    vector_t *v = Calloc(1, sizeof(vector_t));
    v->typesize = typesize;
    v->alloc_size = VECTOR_INIT_SIZE;
    v->data = Malloc(__vector_offset(v, v->alloc_size));
    return v;
}

void vector_set_copy_handler(vector_t *v, __vector_copy_handler vcopy)
{
    __check_vector(v);
    v->vector_copy = vcopy;
}

void vector_set_free_handler(vector_t *v, __vector_free_handler vfree)
{
    __check_vector(v);
    v->vector_free = vfree;
}

void *vector_entry(vector_t *v, size_t index)
{
    __check_vector(v);
    return v->data + __vector_offset(v, index);
}

vector_iterator vector_begin(vector_t *v)
{
    __check_vector(v);
    return __alloc_iterator(v, 0);
}

int vector_next(vector_iterator iter)
{
    if (iter->index + 1 > iter->vector->size - 1)
        return 0;
    iter->index++;
    return 1;
}

void *vector_get(vector_iterator iter)
{
    return iter->vector->data + __vector_offset(iter->vector, iter->index);
}

void vector_free_iterator(vector_iterator iter)
{
    free(iter);
}

void *vector_data(vector_t *v)
{
    __check_vector(v);
    return v->data;
}

int vector_empty(vector_t *v)
{
    __check_vector(v);
    return v->size == 0;
}

size_t vector_size(vector_t *v)
{
    __check_vector(v);
    return v->size;
}

size_t vector_capacity(vector_t *v)
{
    __check_vector(v);
    return v->alloc_size;
}

void vector_reserve(vector_t *v, size_t new_cap)
{
    __check_vector(v);
    if (new_cap > v->alloc_size)
        __vector_realloc(v, new_cap);
}

void vector_shrink_to_fit(vector_t *v)
{
    __check_vector(v);
    __vector_realloc(v, v->size);
}

void vector_insert(vector_t *v, vector_iterator pos, const void *data)
{
    __check_vector(v);
    if (pos) __vector_insert(v, pos->index, data);
}

void vector_erase(vector_t *v, vector_iterator pos)
{
    __check_vector(v);
    __vector_erase(v, pos->index);
}

void vector_push_back(vector_t *v, const void *data)
{
    __check_vector(v);
    __vector_insert(v, v->size, data);
}

void vector_pop_back(vector_t *v)
{
    __check_vector(v);
    if (v->size == 0) return;
    v->size--;
}

void vector_resize(vector_t *v, size_t count)
{
    vector_reserve(v, count);
    if (count < v->size) __vector_free(v, count);
    v->size = count;
}

void vector_clear(vector_t *v)
{
    __check_vector(v);
    __vector_free(v, 0);
    v->size = 0;
}

void vector_swap(vector_t *v, size_t i, size_t j)
{
    __check_vector(v);
    void *tmp = Malloc(v->typesize);
    void *vi = v->data + __vector_offset(v, i);
    void *vj = v->data + __vector_offset(v, j);
    memcpy(tmp, vi, v->typesize);
    memcpy(vi, vj, v->typesize);
    memcpy(vj, tmp, v->typesize);
    free(tmp);
}

void vector_free(vector_t *v)
{
    __check_vector(v);
    __vector_free(v, 0);
    free(v->data);
    free(v);
}
