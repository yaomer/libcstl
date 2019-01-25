#ifndef _GRAPH_H
#define _GRAPH_H

typedef struct {
    char *s;
    int prior;
} Node;

typedef struct _graph {
    Node *gs;
    struct _graph *next;
} _Graph;

typedef struct graph_ {
    _Graph *head;
    _Graph *tail;
    struct graph_ *down;
    int first;
} Graph_;

typedef struct {
    Graph_ *top;
    Graph_ *bottom;
    int first;
} Graph;

Graph   *gh_creat(void);
void    gh_destroy(Graph **);
_Graph  *gh_search(Graph *G, char *s);

void    bfs(Graph *G, char *s, char *t);
void    dfs(Graph *G, char *s, char *t);

#endif  /* _GRAPH_H */
