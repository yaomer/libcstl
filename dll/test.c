#include <stdio.h>
#include "dll.h"

int
main(void)
{
    Link *p = dll_init();

    for (int i = 0; i < 5; i++)
        dll_insert_head(p, i);
    dll_print(p);
    dll_delete(p, 0);
    dll_delete(p, 2);
    dll_delete(p, 4);
    printf("deleted 0, 2, 4:\n");
    dll_print(p);
    dll_destroy(&p);
}
