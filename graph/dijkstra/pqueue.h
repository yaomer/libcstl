#ifndef _PQUEUE_H
#define _PQUEUE_H

#include "graph.h"

#define MAXVAL 101

typedef struct {
    int pqsize;
    Node *ps[MAXVAL];
} Pqueue; 

#define parent(i) (i / 2)
#define left(i) (i * 2)
#define right(i) (i * 2 + 1)
/*
 * 对于宏parent(), left(), right(),
 * 下标从1开始
 */

Pqueue  *pq_init(void);
void    pq_destroy(Pqueue **);
void    pq_push(Pqueue *, Node *);
Node    *pq_pop(Pqueue *);

#endif  /* _PQUEUE_H */
