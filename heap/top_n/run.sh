#/bin/bash

cc number.c
./a.out > input
cc ../heap.c top_n.c
./a.out < input > output
rm a.out
cat output
