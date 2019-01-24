#include <stdio.h>
#include <assert.h>
#include "sll.h"

/*
 * 头插法逆置
 */
void
sll_reverse1(Link **np)
{
    Link *pre, *p;

    assert(np);
    for (pre = NULL; *np; *np = p) {
        p = (*np)->next;
        (*np)->next = pre;
        pre = *np;
    }
    *np = pre;
}

/*
 * 三指针法逆置
 */
void
sll_reverse2(Link **np)
{
    Link *p, *pre = NULL;

    assert(np);
    while (*np) {
        p = *np;
        *np = (*np)->next;
        p->next = pre;
        pre = p;
    }
    *np = p;
}

/*
 * 判断是否有环
 */
Link *
sll_ring(Link *head)
{
    Link *fast, *slow;

    fast = slow = head;
    while (fast && fast->next) {
        fast = fast->next->next;
        slow = slow->next;
        if (fast == slow)
            return fast;
    }
    return NULL;
}

/*
 * 返回中间节点
 */
Link *
sll_mid(Link *head)
{
    Link *fast, *slow;

    fast = slow = head;
    while (fast && fast->next) {
        fast = fast->next->next;
        slow = slow->next;
    }
    return slow;
}

/*
 * 返回倒数第k个节点
 */
Link *
sll_last_k(Link *head, int k)
{
    Link *fast, *slow;

    fast = slow = head;
    while (fast && k-- > 0)
        fast = fast->next;
    while (fast) {
        fast = fast->next;
        slow = slow->next;
    }
    return slow;
}

/*
 * 合并两个有序单链表，类似于归并排序的merge操作
 */
Link *
sll_merge(Link *h1, Link *h2)
{
    Link *p, *head;

    if (!h1 || !h2)
        return h1 ? h1 : h2;

    if (h1->val < h2->val) {
        head = p = h1;
        h1 = h1->next;
    } else {
        head = p = h2;
        h2 = h2->next;
    }

    for ( ; h1 && h2; p = p->next) {
        if (h1->val < h2->val) {
            p->next = h1;
            h1 = h1->next;
        } else {
            p->next = h2;
            h2 = h2->next;
        }
    }
    p->next = h1 ? h1 : h2;

    return head;
}

/*
 * 逆序打印
 */
void
sll_print_r(Link *head)
{
    if (head) {
        sll_print_r(head->next);
        printf("%d\n", head->val);
    }
}
