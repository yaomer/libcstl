#include <stdio.h>
#include "queue.h"

int
main(void)
{
    Queue *Q = queue_init();

    queue_push(Q, 1);
    queue_push(Q, 2);
    queue_push(Q, 3);
    printf("%d ", queue_pop(Q));
    queue_push(Q, 4);
    printf("%d ", queue_pop(Q));
    printf("%d ", queue_pop(Q));
    printf("%d ", queue_pop(Q));
    printf("\n");
    queue_clear(&Q);
}
