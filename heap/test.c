#include <stdio.h>
#include "heap.h"

int
main(void)
{
    Heap *hp = heap_init();
    int a[] = {3, 2, 8, 4, 1, 7, 6, 6, 3, 9};
    int i;

    printf("array: ");
    for (i = 0; i < 10; i++) {
        printf("%d ", a[i]);
        heap_insert(hp, a[i]);
    }
    printf("\n");
    printf("利用heap_insert构建成的一个最大堆:\n");
    for (i = 0; i < 10; i++)
        printf("%d ", hp->val[i + 1]);
    printf("\n");

    printf("the top is %d\n", heap_delete(hp));
    printf("the top is %d\n", heap_delete(hp));

    heap_sort(hp, 8);

    printf("sorted array: ");
    for (i = 1; i <= 8; i++)
        printf("%d ", hp->val[i]);
    printf("\n");

    heap_destroy(&hp);
}
