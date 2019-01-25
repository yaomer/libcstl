#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include "../../common.h"
#include "hash.h"
#include "queue.h"
#include "stack.h"
#include "graph.h"

void
back_path(Hash *hp, char *s)
{
    _Hash *p;

    if ((p = ht_search(hp, s))) {
        back_path(hp, p->pres);
        printf("%s ", s);
    }
}

void
bfs(Graph *G, char *s, char *t)
{
    Hash *visited = ht_init();
    Queue *queue = queue_init();
    Hash *parent = ht_init();  /* 保留(s, pre-s) */

    queue_push(queue, s);
    ht_insert(visited, s, NULL);
    ht_insert(parent, s, NULL);
    while (!is_queue_empty(queue)) {
        _Queue *tq = queue_top(queue);
        _Graph *tg = gh_search(G, tq->s);
        for ( ; tg; tg = tg->next) {
            if (!ht_search(visited, tg->s)) {
                queue_push(queue, tg->s);
                ht_insert(visited, tg->s, NULL);
                ht_insert(parent, tg->s, tq->s);
            }
        }
        assert(tq);
        printf("%s ", tq->s);
        queue_pop(queue);
    }
    printf("\n");
    back_path(parent, t);
    printf("\n");

    queue_clear(&queue);
    ht_destroy(&visited);
    ht_destroy(&parent);
}

void
dfs(Graph *G, char *s, char *t)
{
    Hash *visited = ht_init();
    Hash *parent = ht_init();
    Stack *stack = NULL;

    stack_push(&stack, s);
    ht_insert(visited, s, NULL);
    ht_insert(parent, s, NULL);
    while (!is_stack_empty(stack)) {
        Stack *ts = stack_pop(&stack);
        _Graph *tg = gh_search(G, ts->s);
        for ( ; tg; tg = tg->next) {
            if (!ht_search(visited, tg->s)) {
                stack_push(&stack, tg->s);
                ht_insert(visited, tg->s, NULL);
                ht_insert(parent, tg->s, ts->s);
            }
        }
        printf("%s ", ts->s);
        stack_free(ts);
    }
    printf("\n");
    back_path(parent, t);
    printf("\n");

    stack_clear(&stack);
    ht_destroy(&visited);
    ht_destroy(&parent);
}
