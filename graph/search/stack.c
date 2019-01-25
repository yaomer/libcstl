#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../../common.h"
#include "stack.h"

void
stack_push(Stack **sp, char *s)
{
    Stack *p;

    assert(sp);
    malloc_node(p, Stack);
    malloc_str(p->s, s);
    p->next = *sp;
    *sp = p;
}

void
stack_free(Stack *sp)
{
    assert(sp);
    free(sp->s);
    free(sp);
}

/*
 * 返回栈顶指针sp，然后将sp后移一位，但并不释放sp,
 * 留待稍后释放
 */
Stack *
stack_pop(Stack **sp)
{
    Stack *tmp;

    assert(sp);
    if (!is_stack_empty(*sp)) {
        tmp = *sp;
        *sp = (*sp)->next;
        return tmp;
    } else
        error("stack is empty");
}

void
stack_clear(Stack **sp)
{
    assert(sp);
    while (!is_stack_empty(*sp)) {
        Stack *tmp = (*sp)->next;
        free((*sp)->s);
        free(*sp);
        *sp = tmp;
    }
}

int
is_stack_empty(Stack *sp)
{
    return sp == NULL;
}
