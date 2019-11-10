#ifndef _LIBCSTL_ALLOC_H
#define _LIBCSTL_ALLOC_H

#include <stdlib.h>
#include <assert.h>

static inline void *Malloc(size_t size)
{
    void *ptr = malloc(size);
    assert(ptr);
    return ptr;
}

/* alloc and free up memory */
static inline void *Calloc(size_t count, size_t size)
{
    void *ptr = calloc(count, size);
    assert(ptr);
    return ptr;
}

static inline void *Realloc(void *p, size_t size)
{
    void *ptr = realloc(p, size);
    assert(ptr);
    return ptr;
}

#endif // _LIBCSTL_ALLOC_H
