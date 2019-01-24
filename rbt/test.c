#include <stdio.h>
#include <stdlib.h>
#include "rbt.h"

int
main(void)
{
    rbt_t *root = NULL;
    int i;

    for (i = 0; i < 100; i++)
        rbt_insert(&root, i);
    rbt_print(root);

    for (i = 20; i < 50; i++)
        rbt_delete(&root, i);
    for (i = 60; i < 90; i++) {
        rbt_delete(&root, i);
    }
    rbt_print(root);
}
