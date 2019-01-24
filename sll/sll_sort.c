#include <stdlib.h>
#include <assert.h>
#include "sll.h"

void
sll_insertion_sort(Link **np)
{
    Link **cur;
    Link *new, *key;

    assert(np && *np);
    key = (*np)->next;
    (*np)->next = NULL;

    while (key) {
        cur = np;
        while (*cur && (*cur)->val < key->val)
           cur = &(*cur)->next;
        new = key->next;
        key->next = *cur;
        *cur = key;
        key = new;
    }
}

void
sll_bubble_sort(Link **np)
{
    Link **pre, **cur;
    Link *p, *q;    /*  临时指针  */
    enum {NO, YES};
    int exchange = YES;

    assert(np);
    while (exchange == YES) {
        cur = np;
        exchange = NO;
        while (*cur && (*cur)->next)
            if ((*cur)->val > (*cur)->next->val) {
                exchange = YES;
                pre = &(*cur)->next->next;
                p = *cur;
                q = *pre;
                *pre = *cur;
                *cur = (*cur)->next;
                p->next = q;
                cur = pre;
            } else
                cur = &(*cur)->next;
    }
}

void
sll_selection_sort(Link **np)
{
    Link **cur, **p_min;
    Link *head;

    assert(np);
    for (head = *np; head; np = &(*np)->next) {
        p_min = &head;
        for (cur = &head->next; *cur; cur = &(*cur)->next)
            if ((*cur)->val < (*p_min)->val)
                p_min = cur;
        *np = *p_min;
        *p_min = (*p_min)->next;
        (*np)->next = NULL;
    }
}
