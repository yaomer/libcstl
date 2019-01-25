#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include "../../common.h"
#include "graph.h"

Graph *
gh_init(void)
{
    Graph *G;

    malloc_node(G, Graph);
    bzero(G, sizeof(Graph));

    return G;
}

void
gh_destroy(Graph **G)
{
    Graph_ *p, *pt;
    _Graph *q, *qt;

    assert(G && *G);
    for (p = (*G)->top; p; p = pt) {
        pt = p->down;
        q = p->head;
        while (q) {
            qt = q->next;
            free(q->gs->s);
            free(q->gs);
            free(q);
            q = qt;
        }
        free(p);
    }
    free(*G);
    *G = NULL;
}

_Graph *
gh_search(Graph *G, char *s)
{
    Graph_ *p;

    for (p = G->top; p; p = p->down)
        if (strcmp(p->head->gs->s, s) == 0)
            return p->head;
    return NULL;
}

Graph_ *
gh_insert_head(Graph *G)
{
    Graph_ *p;

    malloc_node(p, Graph_);
    bzero(p, sizeof(Graph_));
    if (G->bottom)
        G->bottom->down = p;
    G->bottom = p;
    if (!G->first) {
        G->top = G->bottom;
        G->first = 1;
    }
    return p;
}

void
gh_insert(Graph_ *G, Node *s)
{
    _Graph *p;

    malloc_node(p, _Graph);
    p->gs = s;
    if (G->tail)
        G->tail->next = p;
    G->tail = p;
    p->next = NULL;
    if (!G->first) {
        G->head = G->tail;
        G->first = 1;
    }
}

Graph *
gh_creat(void)
{
    char line[BUFSIZ];
    char s[BUFSIZ];
    int i, j;
    Graph *G = gh_init();
    Node *ns;

    while (fgets(line, sizeof(line), stdin)) {
        i = 0;
        Graph_ *p = gh_insert_head(G);
        while (line[i] && line[i] != '\n') {
            if (isspace(line[i]) || line[i] == '(' || line[i] == ')') {
                i++;
                continue;
            }
            j = 0;
            malloc_node(ns, Node);
            while (j < BUFSIZ && line[i] != ',')
                s[j++] = line[i++];
            s[j] = '\0';
            ns->prior = atoi(&line[++i]);
            while (line[i] != ')')
                i++;
            malloc_str(ns->s, s);
            gh_insert(p, ns);
        }
    }
    return G;
}

int
main(void)
{
    Graph *G = gh_creat();
    Graph_ *p;
    int i = 0;

    for (p = G->top; p; p = p->down) {
        _Graph *q = p->head;
        while (q) {
            printf("(%s, %d) ", q->gs->s, q->gs->prior);
            q = q->next;
        }
        printf("\n");
    }
}
