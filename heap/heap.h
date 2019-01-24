#ifndef _HEAP_H
#define _HEAP_H

#define MAXVAL 100

typedef struct {
    int heapsize;
    int val[MAXVAL + 1];
} Heap;

#define parent(i) (i / 2)
#define left(i) (i * 2)
#define right(i) (i * 2 + 1)
/*
 * 对于宏parent(), left(), right(),
 * 下标从1开始
 */

Heap    *heap_init(void);
void    heap_destroy(Heap **);
void    build_max_heap(Heap *, int);
void    heap_sort(Heap *, int);
void    heap_insert(Heap *, int);
int     heap_delete(Heap *);

#endif  /* _HEAP_H */
