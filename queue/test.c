#include <stdio.h>
#include "queue_array.h"

int
main(void)
{
    Queue *Q = queue_init();

    printf("queue_push: 1, 2, 3\n");
    queue_push(Q, 1);
    queue_push(Q, 2);
    queue_push(Q, 3);
    printf("top: %d\n", queue_top(Q));
    printf("queue_pop\n");
    queue_pop(Q);
    printf("top: %d\n", queue_top(Q));
    printf("queue_pop\n");
    queue_pop(Q);
    printf("queue_pop\n");
    queue_pop(Q);
    printf("queue_push: 11\n");
    queue_push(Q, 11);
    printf("top: %d\n", queue_top(Q));
    queue_clear(&Q);
}
