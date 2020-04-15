#include <libcstl/hash.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t hash(const void *key)
{
    /* 这里应该选取一个合适的hash算法 */
    size_t h = 0;
    const char *s = (const char *)key;
    for ( ; *s != '\0'; s++)
        h = h * 31 + (*s - '0');
    return h;
}

bool hequal(const void *key1, const void *key2)
{
    return strcmp((char *)key1, (char *)key2) == 0;
}

void hfree(void *key, void *data)
{
    free(key);
}

#define N 1000000

int main(void)
{
    /* pair = <string, null> */
    hash_t *h = hash_init(hash, hequal, hfree);
    char buf[32];
    for (int i = 0; i < N; i++) {
        sprintf(buf, "hello#%d", i);
        char *key = strdup(buf);
        hash_insert(h, key, NULL);
    }
    // 遍历hash
    int map[32] = { 0 }; // 统计对应长度的桶有多少个
    hash_iterator iter = hash_begin(h);
    for ( ; ; ) {
        /* printf("<%s, null> ", (char*)hash_get_key(iter)); */
        int len = hash_bucket_size(h, hash_bucket(h, hash_get_key(iter)));
        map[len]++;
        if (!hash_next(iter)) break;
    }
    hash_free_iterator(iter);
    /* printf("\n"); */
    printf("inserted %d pairs...\n", N);
    printf("hash-buckets: %zu\n", hash_bucket_count(h));
    for (int i = 0; i < sizeof(map)/sizeof(map[0]); i++) {
        if (map[i] > 0)
            printf("<bucket-length:%d, buckets:%d>\n", i, map[i]);
    }
    for (int i = N / 2; i < N; i++) {
        sprintf(buf, "hello#%d", i);
        hash_erase(h, buf);
    }
    printf("deleted %d pairs...\n", N / 2);
    printf("hash-buckets: %zu\n", hash_bucket_count(h));
    hash_free(h);
}
