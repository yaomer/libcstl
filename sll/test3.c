#include <stdio.h>
#include "sll.h"

int
main(void)
{
    int a[] = {2, 4, 9, 1, 2, 8, 5};
    Link *head = NULL;
    int i;

    for (i = 0; i < sizeof(a) / sizeof(a[0]); i++)
        sll_insert_head(&head, a[i]);
    sll_print(head);
    printf("bubble sort:\n");
    sll_bubble_sort(&head);
    sll_print(head);
    sll_destroy(&head);
    putchar('\n');

    for (i = 0; i < sizeof(a) / sizeof(a[0]); i++)
        sll_insert_head(&head, a[i]);
    sll_print(head);
    printf("insertion sort:\n");
    sll_insertion_sort(&head);
    sll_print(head);
    sll_destroy(&head);
    putchar('\n');

    for (i = 0; i < sizeof(a) / sizeof(a[0]); i++)
        sll_insert_head(&head, a[i]);
    sll_print(head);
    printf("selection sort:\n");
    sll_selection_sort(&head);
    sll_print(head);
    sll_destroy(&head);
}
