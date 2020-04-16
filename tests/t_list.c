#include <libcstl/list.h>
#include <stdio.h>
#include <stdlib.h>

int lcomp(const void *k1, const void *k2)
{
    return *(int*)k1 - *(int*)k2;
}

void lfree(void *data)
{
    free(data);
}

int main(void)
{
    list_t *l = list_init(lcomp, lfree);
    for (int i = 0; i < 10; i++) {
        int *p = malloc(sizeof(int));
        *p = i;
        list_push_back(l, p);
    }
    for (int i = 0; i < 10; i++) {
        int *p = malloc(sizeof(int));
        *p = -i;
        list_push_front(l, p);
    }
    list_iterator iter = list_begin(l);
    for ( ; ; ) {
        printf("%d ", *(int*)list_get(iter));
        if (!list_next(iter)) break;
    }
    list_free_iterator(iter);
    printf("\n");
    int val = 0;
    list_erase(l, &val);
    list_erase(l, &val);
    iter = list_begin(l);
    for ( ; ; ) {
        printf("%d ", *(int*)list_get(iter));
        if (!list_next(iter)) break;
    }
    list_free_iterator(iter);
    printf("\n");
    int *p = malloc(sizeof(int));
    *p = 1024;
    list_insert_before(l, list_end(l), p);
    while (!list_empty(l)) {
        printf("%d ", *(int*)list_front(l));
        list_pop_front(l);
    }
    printf("\n");
    list_free(l);
}
