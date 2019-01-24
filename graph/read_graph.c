#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include "../common.h"
#include "graph.h"

/* 读入一个这样描述的图
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

