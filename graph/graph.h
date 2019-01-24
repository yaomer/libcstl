#ifndef _GRAPH_H
#define _GRAPH_H

#define VERTEX 10 

typedef struct graph {
    char *s;
    struct graph *next;
} _Graph;

typedef struct {
    _Graph *head;
    _Graph *tail;
    int first;
} Graph;

typedef struct {
    Graph *graph[VERTEX];
} Graph_;

Graph_  *gh_creat(void);
_Graph  *gh_search(Graph_ *G, char *s);

#endif  /* _GRAPH_H */
