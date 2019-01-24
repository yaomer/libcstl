#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../common.h"
#include "dll.h"

Link *
dll_init(void)
{
    Link *p;

    malloc_node(p, Link);
    p->head = p->tail = NULL;
    p->first = 0;

    return p;
}

void
dll_destroy(Link **dp)
{
    _Link *p, *tmp;

    assert(dp && *dp);
    for (p = (*dp)->head; p; p = tmp) {
        tmp = p->next;
        free(p);
    }
    free(*dp);
    *dp = NULL;
}

static void
_dll_insert_head(Link *dp, _Link *p)
{
    p->last = NULL;
    p->next = dp->head;
    if (dp->head)
        dp->head->last = p;
    dp->head = p;
    if (!dp->first) {
        dp->tail = dp->head;
        dp->first = 1;
    }
}

void
dll_insert_head(Link *dp, int val)
{
    _Link *p;

    assert(dp);
    malloc_node(p, _Link);
    p->val = val;

    _dll_insert_head(dp, p);
}

static void
_dll_insert_tail(Link *dp, _Link *p)
{
    if (dp->tail)
        dp->tail->next = p;
    p->last = dp->tail;
    p->next = NULL;
    dp->tail = p;
    if (!dp->first) {
        dp->head = dp->tail;
        dp->first = 1;
    }
}

void
dll_insert_tail(Link *dp, int val)
{
    _Link *p;

    assert(dp);
    malloc_node(p, _Link);
    p->val = val;

    _dll_insert_tail(dp, p);
}

static void
_dll_insert(Link *dp, _Link *x)
{
    _Link *pre, *p;

    pre = NULL;
    p = dp->head;
    while (p && p->val < x->val) {
        pre = p;
        p = p->next;
    }

    if (p && pre) {   /* 在中间插入 */
        pre->next = x;
        x->last = pre;
        x->next = p;
        p->last = x;
    } else if (p && !pre)   /* 在表头插入 */
        _dll_insert_head(dp, x);
    else if (!p && pre)     /* 在表尾插入 */
        _dll_insert_tail(dp, x);
    else {   /* 在空表中插入一个节点 */
        x->last = x->next = NULL;
        dp->head = dp->tail = x;
        dp->first = 1;
    }
}

void
dll_insert(Link *dp, int val)
{
    _Link *p;

    assert(dp);
    malloc_node(p, _Link);
    p->val = val;

    _dll_insert(dp, p);
}

void
dll_delete(Link *dp, int val)
{
    _Link *p, *pre;

    assert(dp);
    pre = NULL;
    p = dp->head;
    for ( ; p; pre = p, p = p->next)
        if (p->val == val)
            break;

    if (p) {
        if (pre) {
            pre->next = p->next;
            if (p->next)   /* 删除中间节点 */
                p->next->last = p->last;
            else
                dp->tail = pre;   /* 删除表尾 */
        } else   /* 删除表头 */
            dp->head = p->next;
        free(p);
        p = NULL;
    }
}

_Link *
dll_search(Link *dp, int val)
{
    _Link *p;

    assert(dp);
    for (p = dp->head; p; p = p->next)
        if (p->val == val)
            break;
    return p;
}

int
dll_length(Link *dp)
{
    _Link *p;
    int len = 0;

    assert(dp);
    for (p = dp->head; p; p = p->next)
        len++;
    return len;
}

void
dll_print(Link *dp)
{
    _Link *p;

    assert(dp);
    for (p = dp->head; p; p = p->next)
        printf("%d ", p->val);
    printf("\n");
}
