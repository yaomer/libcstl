#include <stdio.h>
#include "hash.h"

int
main(void)
{
    Hash *ht = ht_init();

    ht_insert(ht, "hello");
    ht_insert(ht, "world");
    ht_insert(ht, "you");

    printf("%s\n", ht_search(ht, "hello")->str);
    printf("%s\n", ht_search(ht, "world")->str);
    printf("%s\n", ht_search(ht, "you")->str);

    ht_delete(ht, "world");
    if (!ht_search(ht, "world"))
        printf("deleted ^o^\n");

    ht_destroy(&ht);
}
