#include <stdio.h>
#include "stack_list.h"

int
main(void)
{
    Stack *p = NULL;

    printf("push: 1, 2, 3\n");
    push(&p, 1);
    push(&p, 2);
    push(&p, 3);
    printf("top: %d\n", top(p));
    printf("pop\n");
    pop(&p);
    printf("top: %d\n", top(p));
    printf("clear\n");
    clear(&p);
    pop(&p);
}
