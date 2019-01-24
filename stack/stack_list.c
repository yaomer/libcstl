#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../common.h"
#include "stack_list.h"

void
push(Stack **sp, int new_val)
{
    Stack *p;

    assert(sp);
    malloc_node(p, Stack);
    p->val = new_val;
    p->next = *sp;
    *sp = p;
}

void
pop(Stack **sp)
{
    Stack *tmp;

    assert(sp);
    if (!isempty(*sp)) {
        tmp = (*sp)->next;
        free(*sp);
        *sp = tmp;
    } else
        error("stack is empty");
}

int
top(Stack *sp)
{
    if (!isempty(sp))
        return sp->val;
    else
        error("stack is empty");
}

void
clear(Stack **sp)
{
    assert(sp);
	while (!isempty(*sp))
		pop(sp);
}

int
isfull(Stack *sp)
{
    return 0;
}

int
isempty(Stack *sp)
{
    return sp == NULL;
}
