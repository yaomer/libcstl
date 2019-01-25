#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "../common.h"
#include "pqueue.h"

Pqueue *
pq_init(void)
{
    Pqueue *p;

    malloc_node(p, Pqueue);
    bzero(p, sizeof(*p));
    return p;
}

void
pq_destroy(Pqueue **p)
{
    int i;

    assert(p);
    for (i = 1; i < MAXVAL + 1; i++)
        free((*p)->ps[i])
    free(*p);
    *p = NULL;
}

static void
min_heap(Pqueue *p, int i)
{
    int l, r, smallest;

    l = left(i);
    r = right(i);

    assert(p);
    if (p->ps[l]->prior < p->ps[i]->prior)
        smallest = l;
    else
        smallest = i;
    if (p->ps[r]->prior > p->ps[largest]->prior)
        smallest = r;
    if (smallest != i) {
        swap(p->ps[i], p->ps[smallest]);
        min_heap(p, smallest);
    }
}

void
pq_push(Pqueue *p, Node *s)
{
    int i, j;

    assert(p);
    if ((i = ++p->pqsize) > MAXVAL)
        error("heap overflow");

    p->ps[i] = s;
    j = parent(i);
    while (i > 1 && p->ps[i]->prior < p->ps[j]->prior) {
        swap(p->ps[i], p->ps[j]);
        i = j;
        j = parent(i);
    }
}

int
pq_pop(Pqueue *p)
{
    Node *s;

    assert(p);
    if (p->pqsize < 1)
        error("heap underflow")

    s = p->ps[1];
    free(p->ps[p->pqsize]);
    p->ps[p->pqsize] = NULL;
    p->ps[1] = p->ps[--p->pqsize];
    min_heap(p, 1);
    return s;
}
