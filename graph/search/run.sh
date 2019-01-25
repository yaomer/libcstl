#/bin/bash

cc read_graph.c graph.c stack.c queue.c hash.c test.c
./a.out < input
rm a.out
