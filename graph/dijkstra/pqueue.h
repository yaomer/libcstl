#ifndef _PQUEUE_H
#define _PQUEUE_H

#include "graph.h"

#define MAXVAL 100

typedef struct {
    int pqsize;
    Node *ps[MAXVAL + 1];
} Pqueue; 

#define parent(i) (i / 2)
#define left(i) (i * 2)
#define right(i) (i * 2 + 1)
/*
 * 对于宏parent(), left(), right(),
 * 下标从1开始
 */

Pqueue  *pq_init(void);
void    pq_destroy(pq **);
void    pq_push(pq *, Node *);
int     pq_pop(pq *);

#endif  /* _PQUEUE_H */
