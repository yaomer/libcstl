#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../common.h"
#include "bst.h"

/*
 *      9                 9
 *     / \   insert 12   / \
 *    6  11  ========>  6  11 <-- pre
 *   / \               / \   \
 *  4   7             4   7  12
 */
void
bst_insert(Tnode **rp, int new_val)
{
    Tnode *pre = NULL;   /* p的前驱 */
    Tnode *root;
    Tnode *p;

    assert(rp);
    malloc_node(p, Tnode);

    p->val = new_val;
    p->left = p->right = NULL;

    root = *rp;
    while (root) {  /* 寻找插入位置 */
        pre = root;
        if (p->val < root->val)
            root = root->left;
        else
            root = root->right;
    }
    /* 将p插入到pre后 */
    p->parent = pre;
    if (!pre)
        *rp = p;
    else if (p->val < pre->val)
        pre->left = p;
    else
        pre->right = p;
}

Tnode *
bst_search(Tnode *p, int val)
{
    if (p == NULL || val == p->val)
        return p;
    else if (val < p->val)
        return bst_search(p->left, val);
    else
        return bst_search(p->right, val);
}

/*
 * 拼接两个子树(用以v为根的子树来替换u)
 */

/*
 *   r   v          r
 *  /   / \   ==>  / u->parent->left
 * u   x   y      v
 *               / \
 *              x   y
 */
static void
transplant(Tnode **rp, Tnode *u, Tnode *v)
{
    assert(rp);
    if (!u->parent)
        *rp = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    if (v)
        v->parent = u->parent;
}

Tnode *
bst_minimum(Tnode *p)
{
    if (p)
        while (p->left)
            p = p->left;
    return p;
}

Tnode *
bst_maximum(Tnode *p)
{
    if (p)
        while (p->right)
            p = p->right;
    return p;
}

/***********************************************/
/*
 * 当p->left和p->right都存在时，其为伪删除
 */
Tnode *
bst_fake_delete(Tnode *p, int old_val)
{
    Tnode *tmp;

    if (p == NULL)
        return p;
    else if (old_val < p->val)
        p->left = bst_fake_delete(p->left, old_val);
    else if (old_val > p->val)
        p->right = bst_fake_delete(p->right, old_val);
    else if (p->left && p->right) {   /*  待删除节点有两个孩子  */
        tmp = bst_minimum(p->right);   /*  找到其后继节点  */
        p->val = tmp->val;    /*  用后继节点中的数据来替换待删除节点中的数据  */
        /*  删除后继节点，但待删除节点依然在，因此称为伪删除  */
        p->right = bst_fake_delete(p->right, p->val);
    } else {   /*  待删除节点有0个或1个孩子  */
        tmp = p;
        if (p->left == NULL)
            p = p->right;
        else if (p->right == NULL)
            p = p->left;
        free(tmp);
        tmp = NULL;
    }
    return p;
}
/***********************************************/

/*
 * case 1:  r          r
 *         / \   ==>  / \  不管z(x->right)是否为NULL，都用以z为根的子树来替换x
 *        x   y      z   y
 *         \
 *          z
 * case 2:  r          r
 *         / \   ==>  / \  用以z(x->left)为根的子树来替换x
 *        x   y      z   y
 *       /
 *      z
 * case 3:  r          r
 *         / \        / \
 *        x   y  ==> p   y x的后继是x->right(p)的情况
 *       / \        /
 *      z   p      z
 *          r          r          r
 *         / \        / \        / \
 *        x   y  ==> x   y  ==> p   y  x的后继不是x->right(p)的情况
 *       / \        / \        / \
 *      z   w      z   p      z   w
 *         / \          \        / \
 *        p   q          w      l   q
 *         \            / \
 *          l          l   q
 */

/*
 *      9               9
 *     / \  delete 7   / \
 *    5  11 =======>  6  11
 *   / \             / \
 *  4   7           4   7
 *     / \               \
 *    6   8               8
 */
void
bst_delete(Tnode **rp, int val)
{
    Tnode *x;

    assert(rp);
    /* 找到删除节点 */
    if (!(x = bst_search(*rp, val)))
        return;

    if (!x->left)  /* 只有右孩子或没有孩子 */
        transplant(rp, x, x->right);
    else if (!x->right)  /* 只有左孩子 */
        transplant(rp, x, x->left);
    else {   /* 两个孩子都存在 */
        Tnode *p = bst_minimum(x->right);
        if (p->parent != x) {
            transplant(rp, p, p->right);
            p->right = x->right;
            p->right->parent = p;
        }
        transplant(rp, x, p);
        p->left = x->left;
        p->left->parent = p;
        free(x);
    }
}

/*
 * 返回Tree(x)的后继
 * 如果x->right存在，那么x的后继就是以x->right为根的子树中的最小节点
 * 否则，我们沿树上升，直到x == x->parent->left，此时x->parent即为其后继
 */

/*
 *      9
 *     / \
 *    6  11  6的后继显然是7，7的后继则是9
 *   / \
 *  4   7
 */
Tnode *
bst_successor(Tnode *x)
{
    Tnode *p;

    assert(x);
    if (x->right)
        return bst_minimum(x->right);
    p = x->parent;
    while (p && x == p->right) {
        x = p;
        p = p->parent;
    }
    return p;
}

/*
 * 返回Tree(x)的前驱
 * 查找前驱的过程与查找后继的过程类似
 */
Tnode *
bst_predecessor(Tnode *x)
{
    Tnode *p;

    assert(x);
    if (x->left)
        return bst_maximum(x->left);
    p = x->parent;
    while (p && x == p->left) {
        x = p;
        p = p->parent;
    }
    return p;
}

static void
_travel_pre(Tnode *p)
{
    if (p) {
        printf("%d ", p->val);
        _travel_pre(p->left);
        _travel_pre(p->right);
    }
}

static void
_travel_in(Tnode *p)
{
    if (p) {
        _travel_in(p->left);
        printf("%d ", p->val);
        _travel_in(p->right);
    }
}

static void
_travel_post(Tnode *p)
{
    if (p) {
        _travel_post(p->left);
        _travel_post(p->right);
        printf("%d ", p->val);
    }
}

void
travel_pre(Tnode *p)
{
    _travel_pre(p);
    printf("\n");
}

void
travel_in(Tnode *p)
{
    _travel_in(p);
    printf("\n");
}

void
travel_post(Tnode *p)
{
    _travel_post(p);
    printf("\n");
}
