#ifndef _Link_H
#define _Link_H

typedef struct nlist {
    int val;
    struct nlist *next;
} Link;

void    sll_insert_head(Link **, int);
void    sll_insert(Link **, int);
void    sll_delete(Link **, int);
Link    *sll_search(Link *, int);
int     sll_length(Link *);
void    sll_print(Link *);
void    sll_destroy(Link **);

void    sll_reverse1(Link **);
void    sll_reverse2(Link **);
Link    *sll_ring(Link *);
Link    *sll_mid(Link *);
Link    *sll_last_k(Link *, int);
Link    *sll_merge(Link *, Link *);
void    sll_print_r(Link *);

void    sll_insertion_sort(Link **);
void    sll_bubble_sort(Link **);
void    sll_selection_sort(Link **);

#endif  /* _Link_H */
