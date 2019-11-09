#ifndef _LIBCSTL_ALLOC_H
#define _LIBCSTL_ALLOC_H

#include <stdlib.h>
#include <assert.h>

static inline void *cstl_malloc(size_t size)
{
    void *p = malloc(size);
    assert(p);
    return p;
}

/* alloc and free up memory */
static inline void *cstl_calloc(size_t size)
{
    void *p = calloc(1, size);
    assert(p);
    return p;
}

static inline void *cstl_callocs(size_t count, size_t typesize)
{
    void *p = calloc(count, typesize);
    assert(p);
    return p;
}

#endif // _LIBCSTL_ALLOC_H
