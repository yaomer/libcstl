#include <stdio.h>
#include "graph.h"

int
main(void)
{
    Graph *G = gh_creat();

    bfs(G, "A", "F");
    dfs(G, "A", "F");
}
