#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "../../common.h"
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
    for (i = 1; i < MAXVAL; i++)
        free((*p)->ps[i]);
    free(*p);
    *p = NULL;
}

static void
min_heap(Pqueue *p, int i)
{
    int l, r, smallest;

    l = left(i);
    r = right(i);

    if (l > p->pqsize)
        return;
    if (p->ps[l]->prior < p->ps[i]->prior)
        smallest = l;
    else
        smallest = i;
    if (r > p->pqsize)
        goto next;
    if (p->ps[r]->prior < p->ps[smallest]->prior)
        smallest = r;
next:
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
    if ((i = ++p->pqsize) >= MAXVAL)
        error("heap overflow");

    p->ps[i] = s;
    j = parent(i);
    while (i > 1 && p->ps[i]->prior < p->ps[j]->prior) {
        swap(p->ps[i], p->ps[j]);
        i = j;
        j = parent(i);
    }
}

Node *
pq_pop(Pqueue *p)
{
    Node *s;

    assert(p);
    if (p->pqsize < 1)
        error("heap underflow");

    s = p->ps[1];
    p->ps[1] = p->ps[p->pqsize];
    p->ps[p->pqsize--] = s;
    min_heap(p, 1);
    return s;
}
