#include <libcstl/hash.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * list rbtree skiplist pqueue stack queue与hash的用法类似
 */

size_t hash(const void *key)
{
    return *(size_t*)key;
}

bool hequal(const void *l, const void *r)
{
    return *(size_t*)l == *(size_t*)r;
}

void hfree(void *key, void *value)
{
    free(key);
    free(value);
}

int main(void)
{
    /* pair = <size_t, const char *> */
    hash_t *h = hash_init(hash, hequal, hfree);
    for (int i = 0; i < 100; i++) {
        size_t *key = malloc(sizeof(size_t));
        *key = i;
        char *value = strdup("hello");
        hash_insert(h, key, value);
    }
    hash_iterator iter = hash_begin(h);
    if (!iter) return 1;
    do {
        printf("%zu:%s\n", *(size_t*)hash_get_key(iter), (char*)hash_get_value(iter));
    } while (hash_next(iter));
}
