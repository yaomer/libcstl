#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "skipl.h"

#define _malloc_node(x, type) assert(x = malloc(sizeof(type)))

Skipl *
Skipl_init(void)
{
    Skipl *p;

    _malloc_node(p, Skipl);
    p->top = NULL;
    p->level = 0;

    return p;
}

static unsigned
skipl_random(void)
{
    unsigned level = 1;

    srand(clock());
    while (rand() % 2)
        level++;
    return level;
}

void
skipl_insert(Skipl *sp, int val)
{
    _Skipl *p, *pre, *x, *y;
    _Skipl **dp;
    unsigned level;

    assert(sp);
    p = sp->top;
    level = skipl_random();

    /* level为将要下降的层数 */
    if (level > sp->level) {
        sp->level++;
        level = 0;
    } else
        level = sp->level - level;

}
