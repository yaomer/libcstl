#include <stdio.h>
#include "graph.h"

int
main(void)
{
    Graph *G = gh_creat();

    /* bfs(G, "北京", "西安");
    dfs(G, "北京", "西安"); */
    bfs(G, "A", "F");
    dfs(G, "A", "F");

    gh_destroy(&G);
}
