#include "libsapling/dm/stack.h"

struct edge_storage {
    node_t left;
    node_t right;
    int balance;
};

void *avl__data(const node_t node)
{
    return graph__data(node, sizeof(struct edge_storage));
}


/*
 * The rebalancing code is an adaptation from my university's Data Structures
 * course's slides. I have mostly verified it by examining the resulting step
 * by step graphs from the AVL test.
 */

typedef struct { node_t *ref; int diff; } trail_t;
IMPLEMENT_TYPED_STACK(trail, trail_t, NULL)

struct info_impl {
    cmp_t comparator;
    node_t stack;
};


static
node_t *inorder_next(node_t *ref, node_t *stack)
{
    while (*ref != NULL) {
        trail__insert(stack, (trail_t){ ref });
        node_t node = *ref;
        ref = &node->left;
    }

    return *stack == NULL ? NULL : trail__delete(stack).ref;
}

static
node_t *inorder_first(node_t *ref, node_t *stack)
{
    return *ref == NULL ? ref : inorder_next(ref, stack);
}

static
int next_u(node_t **ref, const struct info_stack *info)
{
    node_t node = **ref;
    struct info_impl *impl = info->impl;

    *ref = inorder_next(&node->right, &impl->stack);
    return *ref != NULL;
}

static
int next_e(node_t **ref, const struct info_stack *info)
{
    node_t node = **ref;
    struct info_insert *user = info->user;
    struct info_impl *impl = info->impl;

    int diff = impl->comparator(avl__data(node), user->data);
    trail__insert(&impl->stack, (trail_t){ *ref, diff });
    *ref = diff <= 0 ? &node->left : &node->right;

    return 1;
}

void avl__access(enum qt qt, node_t *ref, void *info, predicate_t predicate,
    apply_t apply, cmp_t comparator)
{
    struct info_impl impl = { comparator, NULL };
    struct info_stack is = { info, &impl };
    next_t next;
    if (qt == U_QT)
        next = next_u, ref = inorder_first(ref, &impl.stack);
    else // qt == E_QT
        next = next_e;
    graph__access(qt, ref, &is, predicate, apply, next);
}


// AVL rebalancing (BEGIN)

enum {
    INSERT_MODE,
    DELETE_MODE
};

static
void rebalance_left(node_t *node, int *h, int mode)
{
    node_t p = *node;
    node_t a, b, c, b2, c1, c2;
    a = p;
    b = a->left;
    if (b->balance <= 0) {
        b2 = b->right;
        a->left = b2;
        b->right = a;
        *node = b;
        if (b->balance == 0) {
            a->balance = -1;
            b->balance = 1;
            *h = (mode != DELETE_MODE);
        } else {
            a->balance = 0;
            b->balance = 0;
            *h = (mode != INSERT_MODE);
        }
    } else {
        c = b->right;
        c1 = c->left;
        c2 = c->right;
        b->right = c1;
        a->left = c2;
        c->left = b;
        c->right = a;
        *node = c;
        if (c->balance <= 0)
            b->balance = 0;
        else
            b->balance = -1;
        if (c->balance >= 0)
            a->balance = 0;
        else
            a->balance = 1;
        c->balance = 0;
        *h = (mode != INSERT_MODE);
    }
}

static
void balance_left(node_t *node, int *h, int mode)
{
    node_t p = *node;
    if (p->balance == 1) {
        p->balance = 0;
        *h = (mode != INSERT_MODE);
    } else if (p->balance == 0) {
        p->balance = -1;
        *h = (mode != DELETE_MODE);
    } else {
        rebalance_left(node, h, mode);
    }
}

static
void rebalance_right(node_t *node, int *h, int mode)
{
    node_t p = *node;
    node_t a, b, c, b2, c1, c2;
    a = p;
    b = a->right;
    if (b->balance >= 0) {
        b2 = b->left;
        a->right = b2;
        b->left = a;
        *node = b;
        if (b->balance == 0) {
            a->balance = 1;
            b->balance = -1;
            *h = (mode != DELETE_MODE);
        } else {
            a->balance = 0;
            b->balance = 0;
            *h = (mode != INSERT_MODE);
        }
    } else {
        c = b->left;
        c1 = c->left;
        c2 = c->right;
        b->left = c2;
        a->right = c1;
        c->right = b;
        c->left = a;
        *node = c;
        if (c->balance >= 0)
            b->balance = 0;
        else
            b->balance = 1;
        if (c->balance <= 0)
            a->balance = 0;
        else
            a->balance = -1;
        c->balance = 0;
        *h = (mode != INSERT_MODE);
    }
}

static
void balance_right(node_t *node, int *h, int mode)
{
    node_t p = *node;
    if (p->balance == -1) {
        p->balance = 0;
        *h = (mode != INSERT_MODE);
    } else if (p->balance == 0) {
        p->balance = 1;
        *h = (mode != DELETE_MODE);
    } else {
        rebalance_right(node, h, mode);
    }
}

static
void delete_lowest(node_t *node, node_t *plowest, int *h)
{
    node_t p = *node;
    if (p->left != NULL) {
        delete_lowest(&p->left, plowest, h);
        if (*h)
            balance_right(node, h, DELETE_MODE);
    } else {
        *plowest = p;
        *node = p->right;
        *h = 1;
    }
}

// AVL rebalancing (END)


static
void ins_em(node_t *ref, const node_t node, void *impl_ptr)
{
    struct info_impl *impl = impl_ptr;

    node->left = NULL;
    node->right = NULL;
    node->balance = 0;
    *ref = node;

    int h = 1;
    while (impl->stack != NULL && h) {
        trail_t hi = trail__delete(&impl->stack);
        node_t *parent = hi.ref;
        if (hi.diff == 0)
            balance_left(parent, &h, INSERT_MODE);
        else
        if (hi.diff == 1)
            balance_right(parent, &h, INSERT_MODE);
    }
}

static
void insert(node_t *ref, const struct info_stack *info)
{
    graph__insert(ref, info, ins_em, sizeof(struct edge_storage));
}

void avl__insert(node_t *ref, void *info, predicate_t predicate,
    cmp_t comparator)
{
    avl__access(E_QT, ref, info, predicate, insert, comparator);
}


static
void del_em(node_t *ref, const node_t node, void *impl_ptr)
{
    struct info_impl *impl = impl_ptr;

    int h = 0;
    if (node->left == NULL && node->right == NULL) {
        *ref = NULL;
        h = 1;
    } else if (node->left == NULL && node->right != NULL) {
        *ref = node->right;
        h = 1;
    } else if (node->left != NULL && node->right == NULL) {
        *ref = node->left;
        h = 1;
    } else {
        node_t plowest;
        delete_lowest(&node->right, &plowest, &h);
        plowest->left = node->left;
        plowest->right = node->right;
        plowest->balance = node->balance;
        *ref = plowest;
        if (h)
            balance_left(ref, &h, DELETE_MODE);
    }
    while (impl->stack != NULL && h) {
        trail_t hi = trail__delete(&impl->stack);
        node_t *parent = hi.ref;
        if (hi.diff == 0)
            balance_right(parent, &h, DELETE_MODE);
        else
        if (hi.diff == 1)
            balance_left(parent, &h, DELETE_MODE);
    }
}

static
void delete(node_t *ref, const struct info_stack *info)
{
    graph__delete(ref, info, del_em);
}

void avl__delete(node_t *ref, void *info, predicate_t predicate,
    cmp_t comparator)
{
    avl__access(E_QT, ref, info, predicate, delete, comparator);
}


static
void all_access_adapter(node_t *ref, void *info, apply_t apply)
{
    avl__access(U_QT, ref, info, predicate_1, apply, NULL);
}


static
void print_aux0(FILE *stream, const node_t node, fpfdata_t fpfdata,
    void *impl)
{
    fpfdata(stream, avl__data(node));
}

void avl__print(FILE *stream, node_t *ref, fpfdata_t fpfdata)
{
    graph__print(stream, ref, all_access_adapter, print_aux0, fpfdata);
}


void avl__println(FILE *stream, node_t *ref, fpfdata_t fpfdata)
{
    graph__println(stream, ref, all_access_adapter, print_aux0, fpfdata);
}


static
void dump_dot_aux0(FILE *stream, const node_t node, fpfdata_t fpfdata,
    void *impl)
{
    fprintf(stream, "n%p[label=\"", node);
    if (fpfdata != NULL)
        fpfdata(stream, avl__data(node));
    fprintf(stream, " (%d)", node->balance);
    fprintf(stream, "\"];");

    if (node->left != NULL)
        fprintf(stream, "n%p->n%p;", node, node->left);
    else
        fprintf(stream, "n%p->n%p[style=invis];n%p[shape=none;label=\"\"];",
            node, node + 1, node + 1);

    if (node->right != NULL)
        fprintf(stream, "n%p->n%p;", node, node->right);
    else
        fprintf(stream, "n%p->n%p[style=invis];n%p[shape=none;label=\"\"];",
            node, node + 1, node + 1);
}

void avl__dump_dot(FILE *stream, node_t *ref, fpfdata_t fpfdata)
{
    graph__dump_dot(stream, ref, all_access_adapter, dump_dot_aux0, fpfdata,
        "");
}

int avl__length(const node_t *ref)
{
    return graph__length(ref, all_access_adapter);
}
