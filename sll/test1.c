#include <stdio.h>
#include "sll.h"

int
main(void)
{
    int a[] = {3, 4, 1, 6, 2, 5, 7};
    Link *head = NULL;
    int i;

    for (i = 0; i < sizeof(a) / sizeof(a[0]); i++)
        sll_insert(&head, a[i]);
    printf("sll_insert:\n");
    sll_print(head);
    sll_destroy(&head);

    for (i = 0; i < sizeof(a) / sizeof(a[0]); i++)
        sll_insert_head(&head, a[i]);
    printf("sll_insert_head:\n");
    sll_print(head);
    printf("length of list is %d\n", sll_length(head));
    sll_delete(&head, 3);
    sll_delete(&head, 6);
    sll_delete(&head, 7);
    printf("deleted 3, 6, 7:\n");
    sll_print(head);

    if ((head = sll_search(head, 1)))
        printf("%d is in the list\n", head->val);
    else
        printf("not in the list\n");
}
