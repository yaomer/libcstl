/*
 * hash-table的简单实现，采用链接法来解决冲突
 */

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
                free(np);
                np = tmp;
            }
    free(*ht);
    *ht = NULL;
}

unsigned
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

    assert(ht);
    for (np = ht->hashtab[hash(s)]; np; np = np->next)
        if (strcmp(np->str, s) == 0)
            break;
    return np;
}

void
ht_insert(Hash *ht, char *s)
{
    _Hash *np;
    unsigned hashval;

    assert(ht);
    hashval = hash(s);
    malloc_node(np, _Hash);
    malloc_str(np->str, s);
    np->next = ht->hashtab[hashval];
    ht->hashtab[hashval] = np;
}

void
ht_delete(Hash *ht, char *s)
{
    unsigned hashval;
    _Hash *np, *prev;

    assert(ht);
    prev = NULL;
    hashval = hash(s);
    for (np = ht->hashtab[hashval]; np; np = np->next) {
        if (strcmp(np->str, s) == 0)
            break;
        prev = np;
    }
    if (!prev)
        ht->hashtab[hashval] = np->next;
    else
        prev->next = np->next;
    free(np);
    free(np->str);
}
