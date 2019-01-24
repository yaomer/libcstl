#include <stdio.h>
#include "bst.h"

int
main(void)
{
    Tnode *root = NULL;

    int a[] = {1, 4, 2, 1, 6, 9, 3, 2};

    for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++)
        bst_insert(&root, a[i]);
    printf("the original tree:\n");
    travel_in(root);

    printf("the maximum is %d\n", bst_maximum(root)->val);
    printf("the minimum is %d\n", bst_minimum(root)->val);

    printf("deleted 1, 1, 4, 9:\n");
    bst_delete(&root, 4);
    bst_delete(&root, 1);
    bst_delete(&root, 1);
    bst_delete(&root, 9);
    travel_in(root);

    if ((root = bst_search(root, 2)))
        printf("%d is in the tree\n", root->val);
    else
        printf("not in the tree\n");
    return 0;
}
