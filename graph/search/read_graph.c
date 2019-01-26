/*
 * 我们这里用链接法来表示一个图，具体如下：
 *
 * 我们将图中的每个顶点以及它的所有邻接点组织成一条链表，
 * 也就是说对于图G(V,E)，会有V条链表。
 * 每条链表的第一个节点我们可以称之为索引节点，我们还要用另一条链表
 * 来将每个索引节点连接在一起。
 *
 * 这种表示方法的缺点是查找比较耗时。
 * 一种更好的做法是我们可以将索引节点组织为跳跃表或树，甚至对于其邻接点
 * 我们也可以这样做。
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include "../../common.h"
#include "graph.h"

/* 读入一个这样描述的图
 * A B C
 * B A C D
 * C A B D E
 * D B C E F
 * E C D
 * F D
 */

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
            free(q->s);
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
        if (strcmp(p->head->s, s) == 0)
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
gh_insert(Graph_ *G, char *s)
{
    _Graph *p;

    malloc_node(p, _Graph);
    malloc_str(p->s, s);
    if (G->tail)
        G->tail->next = p;
    p->next = NULL;
    G->tail = p;
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

    while (fgets(line, sizeof(line), stdin)) {
        i = 0;
        Graph_ *p = gh_insert_head(G);
        while (line[i] && line[i] != '\n') {
            if (isspace(line[i])) {
                i++;
                continue;
            }
            j = 0;
            while (j < BUFSIZ && !isspace(line[i]))
                s[j++] = line[i++];
            s[j] = '\0';
            gh_insert(p, s);
        }
    }
    return G;
}
