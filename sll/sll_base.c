/*
 * 一些单链表的基本操作，如插入、删除、查找等
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../common.h"
#include "sll.h"

/*
 * 头插法，尾插与之类似
 */
void
sll_insert_head(Link **np, int val)
{
    Link *p;

    assert(np);
    malloc_node(p, Link);
    p->val = val;
    p->next = *np;
    *np = p;
}

/*
 * 有序插入
 */
void
sll_insert(Link **np, int val)
{
    Link *p;

    assert(np);
    malloc_node(p, Link);
    p->val = val;

    while (*np && (*np)->val < p->val)
        np = &(*np)->next;
    p->next = *np;
    *np = p;
}

void
sll_delete(Link **np, int val)
{
    Link *tmp;

    assert(np);
    for ( ; *np; np = &(*np)->next) {
        if ((*np)->val == val) {
            tmp = (*np)->next;
            free(*np);
            *np = tmp;
            break;
        }
    }
}

Link *
sll_search(Link *head, int val)
{
    Link *p;

    for (p = head; p; p = p->next)
        if (p->val == val)
            break;
    return p;
}

int
sll_length(Link *head)
{
    int len = 0;
    Link *p = head;

    while (p) {
        p = p->next;
        len++;
    }
    return len;
}

void
sll_print(Link *head)
{
    Link *p;

    for (p = head; p; p = p->next)
        printf("%d ", p->val);
    printf("\n");
}

void
sll_destroy(Link **np)
{
    Link *tmp;

    assert(np);
    for ( ; *np; *np = tmp) {
        tmp = (*np)->next;
        free(*np);
    }
}
