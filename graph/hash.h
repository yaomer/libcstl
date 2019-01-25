#ifndef _HASH_H
#define _HASH_H

#define HASHSIZE 101

typedef struct hash {
    char *s;
    char *pres;
    struct hash *next;
} _Hash;

typedef struct {
    _Hash *hashtab[HASHSIZE];
} Hash;

Hash    *ht_init(void);
_Hash   *ht_search(Hash *, char *);
void    ht_insert(Hash *, char *, char *);
void    ht_destroy(Hash **);

#endif  /* _HASH_H */