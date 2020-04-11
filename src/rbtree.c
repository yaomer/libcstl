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

#include <stdbool.h>

#include "alloc.h"

enum {
    RED,
    BLACK
};

struct rb_node {
    struct rb_node *left;
    struct rb_node *right;
    struct rb_node *parent;
    void *key;
    void *value;
    size_t size;  /* 子树大小 */
    unsigned char color;
};

typedef int (*__rbtree_comp_handler)(const void *, const void *);
typedef void (*__rbtree_free_handler)(void *, void *);

typedef struct __rbtree {
    size_t size;
    struct rb_node *root;
    struct rb_node *begin;
    struct rb_node *end;
    __rbtree_comp_handler rb_comp;
    __rbtree_free_handler rb_free;
} rbtree_t;

struct __rb_iterator {
    struct rb_node *node;
};

typedef struct __rb_iterator * rb_iterator;

#define __check_rbtree(rb) (assert(rb))

static struct rb_node *__alloc_node(void *key, void *value)
{
    struct rb_node *node = Calloc(1, sizeof(struct rb_node));
    node->key = key;
    node->value = value;
    node->size = 1;
    node->color = RED;
    return node;
}

static void __free_node(rbtree_t *rb, struct rb_node *node)
{
    if (rb->rb_free)
        rb->rb_free(node->key, node->value);
    free(node);
    rb->size--;
}

static rb_iterator __alloc_iterator(struct rb_node *node)
{
    rb_iterator iter = Malloc(sizeof(*iter));
    iter->node = node;
    return iter;
}

static struct rb_node *__rb_find(rbtree_t *rb, struct rb_node *p, const void *key)
{
    if (!p || rb->rb_comp(key, p->key) == 0)
        return p;
    else if (rb->rb_comp(key, p->key) < 0)
        return __rb_find(rb, p->left, key);
    else
        return __rb_find(rb, p->right, key);
}

static struct rb_node *__rb_find_in_delete(rbtree_t *rb, const void *key)
{
    struct rb_node *p = rb->root;
    while (p) {
        if (rb->rb_comp(key, p->key) < 0) {
            p->size--;
            p = p->left;
        } else if (rb->rb_comp(key, p->key) > 0) {
            p->size--;
            p = p->right;
        } else {
            return p;
        }
    }
    return NULL;
}

/* 返回最小节点 */
static struct rb_node *__rb_min(struct rb_node *p)
{
    if (p)
        while (p->left)
            p = p->left;
    return p;
}

/* 返回最大节点 */
static struct rb_node *__rb_max(struct rb_node *p)
{
    if (p)
        while (p->right)
            p = p->right;
    return p;
}

/*
 * 返回node的后继节点
 *      9
 *     / \
 *    6  11  6的后继显然是7，7的后继则是9
 *   / \
 *  4   7
 */
static struct rb_node *__rb_successor(struct rb_node *node)
{
    if (node->right) return __rb_min(node->right);
    struct rb_node *p = node->parent;
    /* if node == p->left, return p */
    while (p && node == p->right) {
        node = p;
        p = p->parent;
    }
    return p;
}

/* 返回node的前驱节点 */
static struct rb_node *__rb_predecessor(struct rb_node *node)
{
    if (node->left) return __rb_max(node->left);
    struct rb_node *p = node->parent;
    while (p && node == p->left) {
        node = p;
        p = p->parent;
    }
    return p;
}

static void __rb_update_begin_and_end_in_insert(rbtree_t *rb, struct rb_node *node)
{
    if (!rb->begin) {
        rb->begin = rb->end = node;
        return;
    }
    if (rb->rb_comp(node->key, rb->begin->key) < 0)
        rb->begin = node;
    if (rb->rb_comp(node->key, rb->end->key) > 0)
        rb->end = node;
}

static void __rb_update_begin_and_end_in_delete(rbtree_t *rb, struct rb_node *node)
{
    if (node == rb->begin)
        rb->begin = __rb_successor(node);
    if (node == rb->end)
        rb->end = __rb_predecessor(node);
}

/* 返回键值大于等于key的最小节点 */
static struct rb_node *__rb_lower_bound(rbtree_t *rb, const void *key)
{
    struct rb_node *p = rb->root;
    struct rb_node *lower = NULL;
    while (p) {
        if (rb->rb_comp(key, p->key) > 0) {
            p = p->right;
        } else if (rb->rb_comp(key, p->key) < 0) {
            lower = p;
            p = p->left;
        } else {
            lower = p;
            break;
        }
    }
    return lower;
}

/* 返回键值小于等于key的最大节点 */
static struct rb_node *__rb_upper_bound(rbtree_t *rb, const void *key)
{
    struct rb_node *p = rb->root;
    struct rb_node *upper = NULL;
    while (p) {
        if (rb->rb_comp(key, p->key) < 0) {
            p = p->left;
        } else if (rb->rb_comp(key, p->key) > 0) {
            upper = p;
            p = p->right;
        } else {
            upper = p;
            break;
        }
    }
    return upper;
}

size_t __rb_order_of_key(rbtree_t *rb, struct rb_node *p, const void *key)
{
    size_t order = 0;
    if (!p) return 0;
    if (p->left) order = p->left->size;
    if (rb->rb_comp(key, p->key) < 0) {
        return __rb_order_of_key(rb, p->left, key);
    } else if (rb->rb_comp(key, p->key) > 0) {
        return order + __rb_order_of_key(rb, p->right, key) + 1;
    } else {
        return order;
    }
}

rb_iterator __rb_find_by_order(rbtree_t *rb, struct rb_node *p, size_t order)
{
    size_t nodes = 0;
    if (!p) return NULL;
    if (p->left) nodes = p->left->size;
    if (nodes > order) {
        return __rb_find_by_order(rb, p->left, order);
    } else if (nodes < order) {
        return __rb_find_by_order(rb, p->right, order - nodes - 1);
    } else {
        return __alloc_iterator(p);
    }
}

static void __rb_update_subtree_size_after_rotate(struct rb_node *x)
{
    x->size = 1;
    if (x->left) x->size += x->left->size;
    if (x->right) x->size += x->right->size;
    if (x->parent) {
        x->parent->size = 1;
        if (x->parent->left)
            x->parent->size += x->parent->left->size;
        if (x->parent->right)
            x->parent->size += x->parent->right->size;
    }
}

/*
 *      x             y
 *     / \    lr     / \
 *    r   y   ==>   x   w
 *       / \       / \
 *      z   w     r   z
 */
static void __rb_left_rotate(rbtree_t *rb, struct rb_node *x)
{
    struct rb_node *p;

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
        rb->root = p;
    p->left = x;
    x->parent = p;
    __rb_update_subtree_size_after_rotate(x);
}

/*
 *      x            r
 *     / \    rr    / \
 *    r   y   ==>  z   x
 *   / \              / \
 *  z   w            w   y
 *
 *  __rb_right_rotate()和__rb_left_rotate()的代码是对称的
 */
static void __rb_right_rotate(rbtree_t *rb, struct rb_node *x)
{
    struct rb_node *p;

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
        rb->root = p;
    p->right = x;
    x->parent = p;
    __rb_update_subtree_size_after_rotate(x);
}

/* A(black) a(red)
 * case 1:
 *         R              R
 *        / \            / \
 *       z   y   ==>    Z   Y    (p->color == RED)
 *      /              /
 *     x              x
 *         R             R          r          X
 *        /   lr(z)     /          /  rr(r)   / \
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
static void __rb_insert_fixup(rbtree_t *rb, struct rb_node *x)
{
    struct rb_node *p;

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
                    __rb_left_rotate(rb, x);
                }
                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                __rb_right_rotate(rb, x->parent->parent);
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
                    __rb_right_rotate(rb, x);
                }
                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                __rb_left_rotate(rb, x->parent->parent);
            }
        }
    }
    rb->root->color = BLACK;
}

static void __rb_undo_modify_of_size(rbtree_t *rb, struct rb_node *x)
{
    (void)__rb_find_in_delete(rb, x->key);
}

/*
 * 插入过程类似于bst的插入过程，只不过多了一步fixup
 */
static void __rb_insert(rbtree_t *rb, struct rb_node *p)
{
    struct rb_node *root = rb->root;
    struct rb_node *pre = NULL;

    __rb_update_begin_and_end_in_insert(rb, p);

    while (root) {
        pre = root;
        if (rb->rb_comp(p->key, root->key) < 0) {
            root->size++;
            root = root->left;
        } else if (rb->rb_comp(p->key, root->key) > 0) {
            root->size++;
            root = root->right;
        } else { /* 更新旧节点的值 */
            if (rb->rb_free) {
                rb->rb_free(NULL, root->value);
                rb->rb_free(p->key, NULL);
            }
            root->value = p->value;
            __rb_undo_modify_of_size(rb, p);
            free(p);
            return;
        }
    }
    p->parent = pre;
    if (pre) {
        if (rb->rb_comp(p->key, pre->key) < 0)
            pre->left = p;
        else
            pre->right = p;
    } else
        rb->root = p;
    __rb_insert_fixup(rb, p);
    rb->size++;
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
static void __rb_transplant(rbtree_t *rb,
        struct rb_node *u, struct rb_node *v)
{
    if (!u->parent)
        rb->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    if (v)
        v->parent = u->parent;
}

static void __rb_delete_fixup(rbtree_t *rb, struct rb_node *x)
{
    struct rb_node *y;

    while (x && x != rb->root && x->color == BLACK) {
        if (x == x->parent->left) {
            if (!(y = x->parent->right))
                break;   /* 隐藏很深的一个BUG: y == NULL */
            if (y->color == RED) {
                y->color = BLACK;
                x->parent->color = RED;
                __rb_left_rotate(rb, x->parent);
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
                    __rb_right_rotate(rb, y);
                    y = x->parent->right;
                }
                y->color = x->parent->color;
                x->parent->color = BLACK;
                y->right->color = BLACK;
                __rb_left_rotate(rb, x->parent);
            }
        } else {
            if (!(y = x->parent->left))
                break;
            if (y->color == RED) {
                y->color = BLACK;
                x->parent->color = RED;
                __rb_right_rotate(rb, x->parent);
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
                    __rb_left_rotate(rb, y);
                    y = x->parent->left;
                }
                y->color = x->parent->color;
                x->parent->color = BLACK;
                y->left->color = BLACK;
                __rb_right_rotate(rb, x->parent);
            }
        }
    }
    if (rb->root)
        rb->root->color = BLACK;
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
static void __rb_delete(rbtree_t *rb, struct rb_node *p)
{
    struct rb_node *x, *y;
    unsigned char origin_color = p->color;

    __rb_update_begin_and_end_in_delete(rb, p);

    if (!p->left) {
        x = p->right;
        __rb_transplant(rb, p, p->right);
    } else if (!p->right) {
        x = p->left;
        __rb_transplant(rb, p, p->left);
    } else {
        y = __rb_min(p->right);
        origin_color = y->color;
        x = y->right;
        if (y->parent != p) {
            __rb_transplant(rb, y, y->right);
            y->right = p->right;
            y->right->parent = y;
        }
        __rb_transplant(rb, p, y);
        y->left = p->left;
        y->left->parent = y;
        y->color = p->color;
    }
    if (origin_color == BLACK)
        __rb_delete_fixup(rb, x);
    __free_node(rb, p);
}

rbtree_t *rb_init(__rbtree_comp_handler rcomp, __rbtree_free_handler rfree)
{
    rbtree_t *rb = Calloc(1, sizeof(rbtree_t));
    rb->rb_comp = rcomp;
    rb->rb_free = rfree;
    return rb;
}

rb_iterator rb_begin(rbtree_t *rb)
{
    __check_rbtree(rb);
    return rb->begin ? __alloc_iterator(rb->begin) : NULL;
}

rb_iterator rb_end(rbtree_t *rb)
{
    __check_rbtree(rb);
    return rb->end ? __alloc_iterator(rb->end) : NULL;
}

bool rb_next(rb_iterator iter)
{
    if (!iter->node) return false;
    iter->node = __rb_successor(iter->node);
    return iter->node != NULL;
}

bool rb_prev(rb_iterator iter)
{
    if (!iter->node) return false;
    iter->node = __rb_predecessor(iter->node);
    return iter->node != NULL;
}

void rb_free_iterator(rb_iterator iter)
{
    free(iter);
}

void *rb_get_key(rb_iterator iter)
{
    return iter->node->key;
}

void *rb_get_value(rb_iterator iter)
{
    return iter->node->value;
}

rb_iterator rb_find(rbtree_t *rb, const void *key)
{
    __check_rbtree(rb);
    struct rb_node *node = __rb_find(rb, rb->root, key);
    return node ? __alloc_iterator(node) : NULL;
}

size_t rb_order_of_key(rbtree_t *rb, const void *key)
{
    __check_rbtree(rb);
    return __rb_order_of_key(rb, rb->root, key);
}

rb_iterator rb_find_by_order(rbtree_t *rb, size_t order)
{
    __check_rbtree(rb);
    return __rb_find_by_order(rb, rb->root, order);
}

void rb_insert(rbtree_t *rb, void *key, void *value)
{
    __check_rbtree(rb);
    __rb_insert(rb, __alloc_node(key, value));
}

void rb_erase(rbtree_t *rb, const void *key)
{
    struct rb_node *p = __rb_find_in_delete(rb, key);
    if (p) __rb_delete(rb, p);
}

bool rb_empty(rbtree_t *rb)
{
    __check_rbtree(rb);
    return rb->size == 0;
}

size_t rb_size(rbtree_t *rb)
{
    __check_rbtree(rb);
    return rb->size;
}

rb_iterator rb_lower_bound(rbtree_t *rb, const void *key)
{
    __check_rbtree(rb);
    struct rb_node *node = __rb_lower_bound(rb, key);
    return node ? __alloc_iterator(node) : NULL;
}

rb_iterator rb_upper_bound(rbtree_t *rb, const void *key)
{
    __check_rbtree(rb);
    struct rb_node *node = __rb_upper_bound(rb, key);
    return node ? __alloc_iterator(node) : NULL;
}

void rb_clear(rbtree_t *rb)
{
    __check_rbtree(rb);
    while (rb->root)
        __rb_delete(rb, rb->root);
}

void rb_free(rbtree_t *rb)
{
    rb_clear(rb);
    free(rb);
}
