#ifndef _BST_H
#define _BST_H

typedef struct tnode {
    int val;
    struct tnode *parent;
    struct tnode *left;
    struct tnode *right;
} Tnode;

void    bst_insert(Tnode **, int);
void    bst_delete(Tnode **, int);
Tnode   *bst_search(Tnode *, int);
Tnode   *bst_maximum(Tnode *);
Tnode   *bst_minimum(Tnode *);
Tnode   *bst_successor(Tnode *);
Tnode   *bst_predecessor(Tnode *);
void    travel_pre(Tnode *);
void    travel_in(Tnode *);
void    travel_post(Tnode *);

#endif  /* _BST_H */
