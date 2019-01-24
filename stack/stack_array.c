/*
 * 用数组实现的简单的堆栈
 *
 * pop()弹出并返回栈顶值 && top()只返回栈顶值
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../common.h"
#include "stack_array.h"

Stack *
init(void)
{
    Stack *p;

    malloc_node(p, Stack);
    bzero(p->val, sizeof(p->val));
    p->sp = -1;

    return p;
}

void
push(Stack *p, int c)
{
    if (!isfull(p))
        p->val[++p->sp] = c;
    else
        error("stack is full");
}

int
pop(Stack *p)
{
    if (!isempty(p))
        return p->val[p->sp--];
    else
        error("Stack is empty");
}

int
top(Stack *p)
{
    if (!isempty(p))
        return p->val[p->sp];
    else
        error("stack is empty");
}

void
clear(Stack **p)
{
    assert(p);
    free(*p);
    *p = NULL;
}

int
isfull(Stack *p)
{
    assert(p);
    return p->sp == MAXVAL - 1;
}

int
isempty(Stack *p)
{
    assert(p);
    return p->sp == -1;
}
