#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#define malloc_node(p, type) (assert(p = malloc(sizeof(type))))

#define malloc_str(s1, s2) do {  \
    if (s2)  \
        assert(s1 = strdup(s2));  \
    else  \
        s1 = s2;  \
} while (0);

#define swap(a, b) do {  \
    typeof(a) tmp = a;  \
    a = b;  \
    b = tmp;  \
} while (0);

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

#define error(s) do { \
    fprintf(stderr, "error: %s\n", s); \
    exit(1); \
} while (0);

#endif  /* _COMMON_H */
