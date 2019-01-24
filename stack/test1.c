#include <stdio.h>
#include "stack_array.h"

int
main(void)
{
    Stack *p = init();

    printf("push: 1, 2, 3\n");
    push(p, 1);
    push(p, 2);
    push(p, 3);
    printf("pop: %d\n", pop(p));
    printf("top: %d\n", top(p));
    printf("pop: %d\n", pop(p));
    printf("top: %d\n", top(p));
    clear(&p);
}
