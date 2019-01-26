#include <stdio.h>
#include "stack.h"

int
main(void)
{
    Stack *p = stack_init();

    stack_push(p, 1);
    stack_push(p, 2);
    stack_push(p, 3);
    printf("%d\n", stack_pop(p));
    printf("%d\n", stack_pop(p));
    stack_push(p, 4);
    printf("%d\n", stack_pop(p));
    stack_clear(&p);
}
