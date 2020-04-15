#include <libcstl/pqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

struct foo {
    char *name; /* 这里不使用name */
    int weight;
};

#define N 10000
#define K 10

#define get(f) (*(struct foo **)(f))

bool pqcomp(const void *f1, const void *f2)
{
    return get(f1)->weight < get(f2)->weight;
}

void pqfree(void *fx)
{
    struct foo *f = (struct foo *)fx;
    /* free(f->name); */
    free(f);
}

struct foo *alloc_foo(int weight)
{
    struct foo *f = malloc(sizeof(*f));
    f->weight = weight;
    /* f->name = ""; */
    return f;
}

/* top k */
int main(void)
{
    int v[N];
    srand(clock());
    for (int i = 0; i < N; i++) {
        v[i] = rand() % N;
    }
    /* 一个最小优先队列 */
    pqueue_t *pq = pq_init(pqcomp, pqfree);
    /* 先将前K个元素入队 */
    for (int i = 0; i < K; i++) {
        struct foo *f = alloc_foo(v[i]);
        pq_push(pq, (void*)&f);
    }
    /* 队列中最后剩下的即为top k */
    for (int i = K; i < N; i++) {
        struct foo *f = get(pq_top(pq));
        if (v[i] > f->weight) { /* 有更大的，入队 */
            f = alloc_foo(v[i]);
            pq_push(pq, (void*)&f);
            pq_pop(pq);
        }
    }
    while (!pq_empty(pq)) {
        printf("%d ", get(pq_top(pq))->weight);
        pq_pop(pq);
    }
    printf("\n");
    pq_free(pq);
}
