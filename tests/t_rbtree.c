#include <libcstl/rbtree.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int rcomp(const void *key1, const void *key2)
{
    return *(int*)key1 - *(int*)key2;
}

void rfree(void *key, void *value)
{
    free(key);
    free(value);
}

#define N 1000

int main(void)
{
    char buf[32];
    srand(clock());
    rbtree_t *r = rb_init(rcomp, rfree);
    for (int i = 0; i < N; i++) {
        int *key = malloc(sizeof(int));
        *key = rand() % N;
        sprintf(buf, "name#%d", *key);
        char *value = strdup(buf);
        rb_insert(r, key, value);
    }
    rb_iterator iter = rb_begin(r);
    for ( ; ; ) {
        printf("<%d, %s>\n", *(int*)rb_get_key(iter), (char*)rb_get_value(iter));
        if (!rb_next(iter)) break;
    }
    rb_free_iterator(iter);
    iter = rb_find_by_order(r, 100);
    int *key = (int*)rb_get_key(iter);
    rb_free_iterator(iter);
    printf("100th is node <%d>\n", *key);
    printf("node <%d> is %zuth\n", *key, rb_order_of_key(r, key));
    int first = 900;
    printf("range 900 - end: ");
    iter = rb_lower_bound(r, &first);
    for ( ; ; ) {
        printf("<%d> ", *(int*)rb_get_key(iter));
        if (!rb_next(iter)) break;
    }
    printf("\n");
}
