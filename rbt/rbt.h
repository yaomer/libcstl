#ifndef _RBT_H
#define _RBT_H

enum {
    RED,
    BLACK
};

typedef struct rbt_node {
    struct rbt_node *left;
    struct rbt_node *right;
    struct rbt_node *parent;
    int val;
    int color;
} rbt_t;

void    rbt_print(rbt_t *);
rbt_t   *rbt_search(rbt_t *, int);
void    rbt_insert(rbt_t **, int);
void    rbt_delete(rbt_t **, int);

#endif  /* _RBT_H */
