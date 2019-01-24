/*
 * 一颗红黑树具有如下性质：
 * 1.每个节点非红即黑
 * 2.根节点是黑色的
 * 3.每个叶节点(NULL)是黑色的
 * 4.如果一个节点是红色的，则它的两个子节点都是黑色的
 * 5.对每个节点，从该节点到其所有后代叶节点的简单路径上，具有相同数目的黑节点
 *
 * 对一颗红黑树进行insert或delete的过程中，性质2 4 5可能遭到破坏。
 * 我们默认将插入的新节点都染成红色，这样可能会违反性质2 4；之所以不染成黑色，是因为
 * 性质5很难维护。而删除一个节点则可能会破坏性质2 4 5。
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../common.h"
#include "rbt.h"

rbt_t *
rbt_search(rbt_t *p, int val)
{
    if (!p || val == p->val)
        return p;
    else if (val < p->val)
        return rbt_search(p->left, val);
    else
        return rbt_search(p->right, val);
}

static void
_rbt_print(rbt_t *p)
{
    if (p) {
        _rbt_print(p->left);
        printf("%d ", p->val);
        _rbt_print(p->right);
    }
}

void
rbt_print(rbt_t *p)
{
    _rbt_print(p);
    printf("\n");
}

/*
 *      x             y
 *     / \    lr     / \
 *    r   y   ==>   x   w
 *       / \       / \
 *      z   w     r   z
 */
static void
rbt_left_rotate(rbt_t **rp, rbt_t *x)
{
    rbt_t *p;

    p = x->right;
    x->right = p->left;
    if (p->left)
        p->left->parent = x;
    p->parent = x->parent;
    if (x->parent) {
        if (x == x->parent->left)
            x->parent->left = p;
        else
            x->parent->right = p;
    } else
        *rp = p;
    p->left = x;
    x->parent = p;
}

/*
 *      x            r
 *     / \    rr    / \
 *    r   y   ==>  z   x
 *   / \              / \
 *  z   w            w   y
 */
static void
rbt_right_rotate(rbt_t **rp, rbt_t *x)
{
    rbt_t *p;

    p = x->left;
    x->left = p->right;
    if (p->right)
        p->right->parent = x;
    p->parent = x->parent;
    if (x->parent) {
        if (x == x->parent->right)
            x->parent->right = p;
        else
            x->parent->left = p;
    } else
        *rp = p;
    p->right = x;
    x->parent = p;
}

/* A(black) a(red)
 * case 1:
 *         R              R
 *        / \            / \
 *       z   y   ==>    Z   Y    (p->color == RED)
 *      /              /
 *     x              x
 *         R             R          r          X
 *        /   ll(z)     /          /  rr(r)   / \
 *       z    ====>    x   ==>    X   ====>  z   r    (!p && x == x->parent->right)
 *        \           /          /
 *         x         z          z
 * case 2:
 *         R              R
 *        / \            / \
 *       y   z   ==>    Y   Z    (p->color == RED)
 *          /               /
 *         x               x
 *         R            R          r              X
 *          \    rr(z)   \          \    lr(r)   / \
 *           z   ====>    x   ==>    X   ====>  r   z  (!p && x == x->parent->left)
 *          /              \          \
 *         x                z          z
 */
static void
rbt_insert_fixup(rbt_t **rp, rbt_t *x)
{
    rbt_t *p;

    while (x->parent && x->parent->color == RED) {
        if (x->parent == x->parent->parent->left) {
            p = x->parent->parent->right;
            if (p && p->color == RED) {
                x->parent->color = BLACK;
                p->color = BLACK;
                x->parent->parent->color = RED;
                x = x->parent->parent;
            } else {
                if (x == x->parent->right) {
                    x = x->parent;
                    rbt_left_rotate(rp, x);
                }
                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                rbt_right_rotate(rp, x->parent->parent);
            }
        } else {   /* 和if的代码是对称的 */
            p = x->parent->parent->left;
            if (p && p->color == RED) {
                x->parent->color = BLACK;
                p->color = BLACK;
                x->parent->parent->color = RED;
                x = x->parent->parent;
            } else {
                if (x == x->parent->left) {
                    x = x->parent;
                    rbt_right_rotate(rp, x);
                }
                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                rbt_left_rotate(rp, x->parent->parent);
            }
        }
    }
    (*rp)->color = BLACK;
}

/*
 * 插入过程类似于bst的插入过程，只不过多了一步fixup
 */
static void
_rbt_insert(rbt_t **rp, rbt_t *p)
{
    rbt_t *root = *rp;
    rbt_t *pre = NULL;

    while (root) {
        pre = root;
        if (p->val < root->val)
            root = root->left;
        else
            root = root->right;
    }
    p->parent = pre;
    if (pre) {
        if (p->val < pre->val)
            pre->left = p;
        else
            pre->right = p;
    } else
        *rp = p;
    p->left = p->right = NULL;
    p->color = RED;
    rbt_insert_fixup(rp, p);
}

void
rbt_insert(rbt_t **rp, int val)
{
    rbt_t *p;

    malloc_node(p, rbt_t);
    p->val = val;

    _rbt_insert(rp, p);
}

/*
 * 拼接两个子树(用以v为根的子树来替换u)
 */

/*
 *   r   v          r
 *  /   / \   ==>  /
 * u   x   y      v
 *               / \
 *              x   y
 */
static void
rbt_transplant(rbt_t **rp, rbt_t *u, rbt_t *v)
{
    if (!u->parent)
        *rp = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    if (v)
        v->parent = u->parent;
}

rbt_t *
rbt_minimum(rbt_t *p)
{
    if (p)
        while (p->left)
            p = p->left;
    return p;
}

rbt_t *
rbt_maximum(rbt_t *p)
{
    if (p)
        while (p->right)
            p = p->right;
    return p;
}

static void
rbt_delete_fixup(rbt_t **rp, rbt_t *x)
{
    rbt_t *y;

    while (x && x != *rp && x->color == BLACK) {
        if (x == x->parent->left) {
            if (!(y = x->parent->right))
                break;   /* 隐藏很深的一个BUG: y == NULL */
            if (y->color == RED) {
                y->color = BLACK;
                x->parent->color = RED;
                rbt_left_rotate(rp, x->parent);
                y = x->parent->right;
            }
            if ((!y->left  || y->left->color == BLACK) &&
                (!y->right || y->right->color == BLACK)) {
                y->color = RED;
                x = x->parent;
            } else {
                if (!y->right || y->right->color == BLACK) {
                    y->left->color = BLACK;
                    y->color = RED;
                    rbt_right_rotate(rp, y);
                    y = x->parent->right;
                }
                y->color = x->parent->color;
                x->parent->color = BLACK;
                y->right->color = BLACK;
                rbt_left_rotate(rp, x->parent);
            }
        } else {
            if (!(y = x->parent->left))
                break;
            if (y->color == RED) {
                y->color = BLACK;
                x->parent->color = RED;
                rbt_right_rotate(rp, x->parent);
                y = x->parent->left;
            }
            if ((!y->left  || y->left->color == BLACK) &&
                (!y->right || y->right->color == BLACK)) {
                y->color = RED;
                x = x->parent;
            } else {
                if (!y->left || y->left->color == BLACK) {
                    y->right->color = BLACK;
                    y->color = RED;
                    rbt_left_rotate(rp, y);
                    y = x->parent->left;
                }
                y->color = x->parent->color;
                x->parent->color = BLACK;
                y->left->color = BLACK;
                rbt_right_rotate(rp, x->parent);
            }
        }
    }
    if (*rp)
        (*rp)->color = BLACK;
}

/*
 * case 1:
 *         R           R
 *        / \         / \
 *       Z   W  ==>  Z   W
 *      /
 *     p
 * case 2: 类似case 1
 * case 3:
 *         R           R           R
 *        / \         / \         / \
 *       p   W  ==>  p   W  ==>  s   W
 *      / \         / \         / \
 *     L   Z       L   s       L   Z
 *        / \           \           \
 *       s   t           Z           t
 *                        \
 *                         t
 */
static void
_rbt_delete(rbt_t **rp, rbt_t *p)
{
    rbt_t *x, *y;
    int origin_color;

    origin_color = p->color;
    if (!p->left) {
        x = p->right;
        rbt_transplant(rp, p, p->right);
    } else if (!p->right) {
        x = p->left;
        rbt_transplant(rp, p, p->left);
    } else {
        y = rbt_minimum(p->right);
        origin_color = y->color;
        x = y->right;
        if (y->parent != p) {
            rbt_transplant(rp, y, y->right);
            /* 切记这里y->right不能换作x，因为x可能为NULL */
            y->right = p->right;
            y->right->parent = y;
        }
        rbt_transplant(rp, p, y);
        y->left = p->left;
        y->left->parent = y;
        y->color = p->color;
    }
    if (origin_color == BLACK)
        rbt_delete_fixup(rp, x);
    free(p);
}

void
rbt_delete(rbt_t **rp, int val)
{
    rbt_t *p;

    if (!(p = rbt_search(*rp, val)))
        return;

    _rbt_delete(rp, p);
}
