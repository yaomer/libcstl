#include <stdio.h>
#include <assert.h>
#include "../../common.h"
#include "pqueue.h"

/*
 * 测试优先队列
 */
int
main(void)
{
    Pqueue *pq = pq_init();
    Node *s[6];
    int prior[] = {5, 2, 1, 6, 3, 0};
    char *p[] = {"A", "B", "C", "D", "E", "F"};
    int i;

    for (i = 0; i < 6; i++) {
        malloc_node(s[i], Node);
        s[i]->prior = prior[i];
        malloc_str(s[i]->s, p[i]);
        pq_push(pq, s[i]);
        printf("(%s, %d) ", s[i]->s, s[i]->prior);
    }
    printf("\n");

    for (i = 0; i < 6; i++) {
        Node *t = pq_pop(pq);
        printf("(%s, %d) ", t->s, t->prior);
    }
    printf("\n");

    pq_destroy(&pq);
}
