#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include "../common.h"
#include "hash.h"
#include "queue.h"
#include "graph.h"
/*
 * A B C
 * B A C D
 * C A B D E
 * D B C E F
 * E C D
 * F D
 */

Graph_ *
gh_init(void)
{
    Graph_ *G;
    int i;

    malloc_node(G, Graph_);
    for (i = 0; i < VERTEX; i++) {
        malloc_node(G->graph[i], Graph);
        bzero(G->graph[i], sizeof(Graph));
    }

    return G;
}

_Graph *
gh_search(Graph_ *G, char *s)
{
    int i;

    for (i = 0; i < VERTEX; i++)
        if (strcmp(s, G->graph[i]->head->s) == 0)
            return G->graph[i]->head;
    return NULL;
}

void
gh_insert(Graph *H, char *s)
{
    _Graph *G;

    malloc_node(G, _Graph);
    malloc_str(G->s, s);
    if (H->tail)
        H->tail->next = G;
    G->next = NULL;
    H->tail = G;

    if (!H->first) {
        H->head = H->tail;
        H->first = 1;
    }
}

Graph_ *
gh_creat(void)
{
    char line[BUFSIZ];
    char s[BUFSIZ];
    int i, j, k;
    Graph_ *G;

    G = gh_init();
    for (i = 0; i < VERTEX; i++) {
        fgets(line, sizeof(line), stdin);
        j = 0;
        while (line[j] && line[j] != '\n') {
            if (isspace(line[j])) {
                j++;
                continue;
            }
            k = 0;
            while (k < BUFSIZ && !isspace(line[j]))
                s[k++] = line[j++];
            s[k] = '\0';
            gh_insert(G->graph[i], s);
        }
    }
    return G;
}

void
bfs(Graph_ *G, char *s, char *t)
{
    Hash *visited = ht_init();
    Queue *queue = queue_init();
    Hash *parent = ht_init();

    queue_push(queue, s);
    ht_insert(visited, s, NULL);
    ht_insert(parent, s, NULL);
    while (!is_queue_empty(queue)) {
        _Queue *tq = queue_top(queue);
        _Graph *tg = gh_search(G, tq->s);
        while (tg) {
            if (!ht_search(visited, tg->s)) {
                queue_push(queue, tg->s);
                /* ht_insert(visited, tg->s); */
                ht_insert(visited, tg->s, NULL);
                ht_insert(parent, tg->s, tq->s);
            }
            tg = tg->next;
        }
        /* printf("%s ", tq->s); */
        queue_pop(queue);
    }

    _Hash *tmp;
    while ((tmp = ht_search(parent, t))) {
        printf("%s ", t);
        t = tmp->pres;
    }
    printf("\n");
    queue_clear(&queue);
    ht_destroy(&visited);
    ht_destroy(&parent);
}

int
main(void)
{
    Graph_ *G = gh_creat();

    bfs(G, "A", "E");
}
