#include <stdio.h>
#include "sll.h"

#define LEN(a, b) (sizeof(a) / sizeof(b))

int
main(void)
{
    int a[] = {2, 3, 8, 2};
    int b[] = {4, 1, 5, 3};
    Link *h1, *h2;
    int i;

    h1 = h2 = NULL;
    for (i = 0; i < LEN(a, a[0]); i++)
        sll_insert(&h1, a[i]);
    printf("list 1 is:\n");
    sll_print(h1);
    for (i = 0; i < LEN(b, b[0]); i++)
        sll_insert(&h2, b[i]);
    printf("list 2 is:\n");
    sll_print(h2);
    printf("merge list 1 and list 2:\n");
    sll_print(h1 = sll_merge(h1, h2));
    printf("reverse print:\n");
    sll_print_r(h1);

    sll_reverse1(&h1);
    printf("sll_reverse1:\n");
    sll_print(h1);
    sll_reverse2(&h1);
    printf("sll_reverse2:\n");
    sll_print(h1);


    h2 = sll_last_k(h1, 1);
    h2->next = h1->next;
    if (sll_ring(h1))
        printf("the list has ring\n");
    else
        printf("the list hasn't ring\n");
}
