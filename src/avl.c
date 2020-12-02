#include "avl.h"
#include "stack.h"

struct edge_storage {
    void *left;
    void *right;
    int balance;
};

size_t avl__edge_storage(void)
{
    return sizeof(struct edge_storage);
}


struct info_impl {
    void *stack;
    int (*choose)(void **, void *);
};

struct info_ref_choice {
    void **ref;
    int choice;
};

struct info_insert_ref {
    size_t size;
    struct info_ref_choice rc;
};


static
void next_e(void ***ref, void *info)
{
    struct infostack *is = info;
    struct info_impl *impl = is->impl;
    struct edge_storage *node = **ref;
    int choice = impl->choose(*ref, info);
    struct info_insert_ref iir = {
        sizeof(struct info_ref_choice),
        { *ref, choice }
    };
    stack__insert(&impl->stack, &iir);
    *ref = choice ? &node->right : &node->left;
}


static
void expand_leftmost_children(void **ref, void **stack)
{
    while (*ref != NULL) {
        struct info_insert_ref iir = { sizeof(struct info_ref_choice), ref };
        stack__insert(stack, &iir);
        struct edge_storage *node = *ref;
        ref = &node->left;
    }
}

static
void next_u(void ***ref, void *info)
{
    struct infostack *is = info;
    struct info_impl *impl = is->impl;
    struct edge_storage *node = **ref;

    expand_leftmost_children(&node->right, &impl->stack);
    if (impl->stack == NULL) {
        *ref = &impl->stack; // needs reference to NULL
    } else {
        *ref = *(void **)stack__access(&impl->stack);
        stack__delete(&impl->stack);
    }
}


// AVL rebalancing (BEGIN)

enum {
    INSERT_MODE,
    DELETE_MODE
};

static
void rebalance_left(void **node, int *h, int mode)
{
    struct edge_storage *p = (struct edge_storage *)(*node);
    struct edge_storage *a, *b, *c, *b2, *c1, *c2;
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
void balance_left(void **node, int *h, int mode)
{
    struct edge_storage *p = (struct edge_storage *)(*node);
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
void rebalance_right(void **node, int *h, int mode)
{
    struct edge_storage *p = (struct edge_storage *)(*node);
    struct edge_storage *a, *b, *c, *b2, *c1, *c2;
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
void balance_right(void **node, int *h, int mode)
{
    struct edge_storage *p = (struct edge_storage *)(*node);
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
void delete_lowest(void **node, struct edge_storage **plowest, int *h)
{
    struct edge_storage *p = (struct edge_storage *)(*node);
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
void insert_edge_management(void **ref, struct edge_storage *node, void *info)
{
    node->left = NULL;
    node->right = NULL;
    node->balance = 0;
    *ref = node;

    int h = 1;
    struct infostack *is = info;
    struct info_impl *impl = is->impl;

    while (impl->stack != NULL && h) {
        struct info_ref_choice *rc = stack__access(&impl->stack);
        void **parent = rc->ref;
        if (rc->choice == 0)
            balance_left(parent, &h, INSERT_MODE);
        else
        if (rc->choice == 1)
            balance_right(parent, &h, INSERT_MODE);
        stack__delete(&impl->stack);
    }
}

static
void insert(void **ref, void *info)
{
    graph__insert(ref, info, sizeof(struct edge_storage),
        insert_edge_management);
}


static
void delete_edge_management(void **ref, struct edge_storage *node, void *info)
{
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
        struct edge_storage *plowest;
        delete_lowest(&node->right, &plowest, &h);
        plowest->left = node->left;
        plowest->right = node->right;
        plowest->balance = node->balance;
        *ref = plowest;
        if (h)
            balance_left(ref, &h, DELETE_MODE);
    }

    struct infostack *is = info;
    struct info_impl *impl = is->impl;

    while (impl->stack != NULL && h) {
        struct info_ref_choice *rc = stack__access(&impl->stack);
        void **parent = rc->ref;
        if (rc->choice == 0)
            balance_right(parent, &h, DELETE_MODE);
        else
        if (rc->choice == 1)
            balance_left(parent, &h, DELETE_MODE);
        stack__delete(&impl->stack);
    }
}

static
void delete(void **ref, void *info)
{
    graph__delete(ref, info, delete_edge_management);
}


void avl__insert(void **ref, void *info, choose_fn choose)
{
    struct info_impl impl = { NULL, choose };
    struct infostack is = { info, &impl };
    graph__eloop(ref, &is, match_0, next_e, insert);
}

void avl__delete(void **ref, void *info, choose_fn choose, match_fn match)
{
    struct info_impl impl = { NULL, choose };
    struct infostack is = { info, &impl };
    graph__eloop(ref, &is, match, next_e, delete);
}

void avl__access(int qt, void **ref, void *info,
    choose_fn choose, match_fn match, apply_fn apply)
{
    struct info_impl impl = { NULL, choose };
    struct infostack is = { info, &impl };
    switch (qt) {
    case U_QT:
        // init stack
        expand_leftmost_children(ref, &impl.stack);
        if (impl.stack == NULL) {
            ref = &impl.stack; // needs reference to NULL
        } else {
            ref = *(void **)stack__access(&impl.stack);
            stack__delete(&impl.stack);
        }

        graph__uloop(ref, &is, match, next_u, apply);
        break;
    case E_QT:
        graph__eloop(ref, &is, match, next_e, apply);
        break;
    default:
        break;
    }
}


static
void apply_dump_dot(void **ref, void *info)
{
    struct edge_storage *node = *ref;
    struct infostack *is = info;
    struct info_dump_dot *i = is->user;
    FILE *fd = i->fd;
    void (*fpd)(FILE *, void **) = i->fpd;
    fprintf(fd, "n%p[label=\"", node);
    if (fpd != NULL)
        fpd(fd, ref);
    fprintf(fd, " (%d)", node->balance);
    fprintf(fd, "\"];");
    if (node->left != NULL)
        fprintf(fd, "n%p->n%p;", node, node->left);
    if (node->right != NULL)
        fprintf(fd, "n%p->n%p;", node, node->right);
}

void avl__dump_dot(FILE *fd, void **ref, void (*fpd)(FILE *fd, void **ref))
{
    fprintf(fd, "digraph{");
    struct info_dump_dot info = { fd, fpd };
    avl__access(U_QT, ref, &info, NULL, match_1, apply_dump_dot);
    fprintf(fd, "}\n");
}
