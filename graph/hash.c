#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../common.h"
#include "hash.h"

Hash *
ht_init(void)
{
    Hash *ht;

    malloc_node(ht, Hash);
    bzero(ht->hashtab, sizeof(ht->hashtab));

    return ht;
}

void
ht_destroy(Hash **ht)
{
    _Hash *np, *tmp;

    assert(ht && *ht);
    for (int i = 0; i < HASHSIZE; i++)
        if ((np = (*ht)->hashtab[i]))
            while (np) {
                tmp = np->next;
                free(np->s);
                free(np->pres);
                free(np);
                np = tmp;
            }
    free(*ht);
    *ht = NULL;
}

static unsigned
hash(char *s)
{
    unsigned hashval;

    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}

_Hash *
ht_search(Hash *ht, char *s)
{
    _Hash *np;

    if (!s)
        return NULL;

    assert(ht);
    for (np = ht->hashtab[hash(s)]; np; np = np->next)
        if (strcmp(np->s, s) == 0)
            break;
    return np;
}

void
ht_insert(Hash *ht, char *s, char *pres)
{
    _Hash *np;
    unsigned hashval;

    assert(ht);
    if ((np = ht_search(ht, s))) {
        free(np->pres);
        malloc_str(np->pres, pres);
    } else {
        hashval = hash(s);
        malloc_node(np, _Hash);
        malloc_str(np->s, s);
        malloc_str(np->pres, pres);
        np->next = ht->hashtab[hashval];
        ht->hashtab[hashval] = np;
    }
}
