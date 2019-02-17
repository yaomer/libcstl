#ifndef _DLL_H
#define _DLL_H

typedef struct nlist {
    int val;
    struct nlist *next;
    struct nlist *last;
} _Link;

typedef struct {
    _Link *head;
    _Link *tail;
} Link;

Link    *dll_init(void);
void    dll_destroy(Link **);
void    dll_insert_head(Link *, int);
void    dll_insert_tail(Link *, int);
void    dll_insert(Link *, int);
void    dll_delete(Link *, int);
_Link   *dll_search(Link *, int);
int     dll_length(Link *);
void    dll_print(Link *);

#endif  /* _DLL_H */
