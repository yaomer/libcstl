#include <stdio.h>
#include "heap.h"

int
main(void)
{
    Heap *hp = heap_init();
    int a[] = {3, 2, 8, 4, 1, 7, 6, 6, 3, 9};
    int i;

    for (i = 0; i < 10; i++)
        heap_insert(hp, a[i]);

    printf("array:");
    for (i = 0; i < 10; i++)
        printf("%3d", a[i]);
    printf("\n");

    printf("the max value = %3d\n", hp->val[1]);
    heap_delete(hp);
    printf("after heap_delete, the max value = %3d\n", hp->val[1]);
    heap_insert(hp, 9);

    heap_sort(hp, 10);

    printf("sorted array:");
    for (i = 1; i <= 10; i++)
        printf("%3d", hp->val[i]);
    printf("\n");

    heap_destroy(&hp);
}
