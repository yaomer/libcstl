/*
 * 用两个队列实现一个栈，思路如下：
 *
 * sp->push和sp->pop都为空的时候，我们选用sp->push压入元素；否则的话，
 * 我们用其中不为空的那个压入元素。
 * 当要出栈的时候，我们让两个队列中非空的一个只留下一个元素，其余全部出队，
 * 放到另一个队列中，留下的那个元素就是我们需要弹出的。
 *
 * 也就是说对于stack_pop操作，我们需要在两个队列之间不停的转移元素。
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../../common.h"
#include "../queue_array.h"
#include "stack.h"

Stack *
stack_init(void)
{
    Stack *sp;

    malloc_node(sp, Stack);
    sp->push = queue_init();
    sp->pop = queue_init();

    return sp;
}

void
stack_push(Stack *sp, int val)
{
    if (!is_queue_empty(sp->push)) {
        if (!is_queue_full(sp->push))
            queue_push(sp->push, val);
        else
            error("stack is full");
    } else if (!is_queue_empty(sp->pop)) {
        if (!is_queue_full(sp->pop))
            queue_push(sp->pop, val);
        else
            error("stack is full");
    } else  /* 两个都为空，随便选一个 */
        queue_push(sp->push, val);
}

int
stack_pop(Stack *sp)
{
    int val;

    if (!is_queue_empty(sp->push)) {
        while (!is_queue_empty(sp->push)) {
            val = queue_pop(sp->push);
            /* 保留sp->push中最后一个元素，不压入sp->pop中 */
            if (is_queue_empty(sp->push))
                break;
            if (!is_queue_full(sp->pop))
                queue_push(sp->pop, val);
            else
                error("stack is full");
        }
        return val;
    } else if (!is_queue_empty(sp->pop)) {
        while (!is_queue_empty(sp->pop)) {
            val = queue_pop(sp->pop);
            if (is_queue_empty(sp->pop))
                break;
            if (!is_queue_full(sp->push))
                queue_push(sp->push, val);
            else
                error("stack is full");
        }
        return val;
    } else
        error("stack is empty");
}

int
is_stack_full(Stack *sp)
{
    assert(sp);
    return is_queue_full(sp->push) || is_queue_full(sp->pop);
}

int
is_stack_empty(Stack *sp)
{
    assert(sp);
    return is_queue_empty(sp->push) && is_queue_empty(sp->pop);
}

void
stack_clear(Stack **sp)
{
    assert(sp && *sp);
    queue_clear(&(*sp)->push);
    queue_clear(&(*sp)->pop);
    free(*sp);
    *sp = NULL;
}
