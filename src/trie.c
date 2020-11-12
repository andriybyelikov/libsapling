#include <string.h>
#include "graph.h"
#include "avl.h"
#include "stack.h"

struct edge_storage {
    void *edge_map; // dynamic edges of the node attributed with char
    int in;         // key pertains to collection
};

struct mapping {
    char byte;
    void *node;
};

size_t trie__edge_storage(void)
{
    return sizeof(struct edge_storage);
}


struct info_impl {
    void *stack;
    const char *key;
    int ki;
};

struct info_insert_mapping {
    size_t size;
    struct mapping mapping;
};


static
int match_e(void **ref, void *info)
{
    struct infostack *is = info;
    struct info_impl *impl = is->impl;
    return strlen(impl->key) == impl->ki;
}


// dynamic node edges management (BEGIN)

static
int mapping_choose(void **ref, void *info)
{
    struct infostack *is = info;
    struct info_insert_mapping *i = is->user;
    void *node = *ref;
    struct mapping *m = node__data(node, avl__edge_storage());
    char a = i->mapping.byte;
    char b = m->byte;
    return a < b ? 0 : 1;
}

static
int mapping_match(void **ref, void *info)
{
    struct infostack *is = info;
    struct info_insert_mapping *i = is->user;
    void *node = *ref;
    struct mapping *m = node__data(node, avl__edge_storage());
    char a = i->mapping.byte;
    char b = m->byte;
    return a == b;
}

static
void mapping_getref(void **ref, void *info)
{
    if (*ref != NULL) {
        struct infostack *is = info;
        struct info_insert_mapping *i = is->user;
        void *node = *ref;
        struct mapping *m = node__data(node, avl__edge_storage());
        i->mapping.node = &m->node;
    }
}

// dynamic node edges management (END)

static
void insert_edge_management_not_in(void **ref, struct edge_storage *node,
    void *info)
{
    node->edge_map = NULL;
    node->in = 0;
    *ref = node;
}

static
void next_insert(void ***ref, void *info)
{
    struct infostack *is = info;
    struct info_impl *impl = is->impl;
    struct edge_storage *node = **ref;
    struct info_insert_mapping iim = {
        sizeof(struct info_insert_mapping),
        { impl->key[impl->ki], NULL }
    };
    // search edge attributed to the current character
    avl__access(E_QT, &node->edge_map, &iim, mapping_choose, mapping_match,
        mapping_getref);
    // if it does not exist insert it and retrieve the reference to its edge
    if (iim.mapping.node == NULL) {
        avl__insert(&node->edge_map, &iim, mapping_choose);
        avl__access(E_QT, &node->edge_map, &iim, mapping_choose, mapping_match,
            mapping_getref);
    }
    *ref = iim.mapping.node;
    // if next node does not exist create it in advance with its key not
    // pertaining to the collection
    if (**ref == NULL) {
        graph__insert(*ref, info, sizeof(struct edge_storage),
            insert_edge_management_not_in);
    }
    impl->ki++;
}

struct info_ref {
    void **ref;
};

struct info_insert_ref {
    size_t size;
    struct info_ref r;
};

static
void next_delete(void ***ref, void *info)
{
    struct infostack *is = info;
    struct info_impl *impl = is->impl;
    struct edge_storage *node = **ref;

    struct info_insert_ref iir = {
        sizeof(struct info_ref),
        { *ref }
    };
    stack__insert(&impl->stack, &iir);

    struct info_insert_mapping iim = {
        sizeof(struct info_insert_mapping),
        { impl->key[impl->ki], NULL }
    };
    avl__access(E_QT, &node->edge_map, &iim, mapping_choose, mapping_match,
        mapping_getref);
    *ref = iim.mapping.node;
    impl->ki++;
}


static
void expand_children(void **ref, void *info)
{
    if (*ref != NULL) {
        void *node = *ref;
        struct mapping *m = node__data(node, avl__edge_storage());
        struct info_insert_ref iir = { sizeof(struct info_ref), { &m->node } };
        struct infostack *is = info;
        struct info_impl *impl = is->user;
        stack__insert(&impl->stack, &iir);
    }
}

static
void next_u(void ***ref, void *info)
{
    struct infostack *is = info;
    struct info_impl *impl = is->impl;
    struct edge_storage *node = **ref;

    avl__access(U_QT, &node->edge_map, impl, NULL, match_1, expand_children);
    if (impl->stack == NULL)
        *ref = &node->edge_map; // needs reference to NULL
    else
        *ref = *(void **)stack__access(&impl->stack);
    stack__delete(&impl->stack);
}


static
void next_e(void ***ref, void *info)
{
    struct infostack *is = info;
    struct info_impl *impl = is->impl;
    struct edge_storage *node = **ref;
    struct info_insert_mapping iim = {
        sizeof(struct info_insert_mapping),
        { impl->key[impl->ki], NULL }
    };
    avl__access(E_QT, &node->edge_map, &iim, mapping_choose, mapping_match,
        mapping_getref);
    *ref = iim.mapping.node;
    impl->ki++;
}


static
void insert_edge_management(void **ref, struct edge_storage *node, void *info)
{
    node->edge_map = NULL;
    node->in = 1;
    *ref = node;
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
    *ref = NULL;

    struct infostack *is = info;
    struct info_impl *impl = is->impl;

    
    while (impl->stack != NULL) {
        char c = impl->key[--impl->ki];
        void **r = *(void ***)stack__access(&impl->stack);
        struct edge_storage *node = *r;

        // delete character edge
        struct info_insert_mapping imm = {
            sizeof(struct mapping),
            { c, NULL }
        };
        avl__delete(&node->edge_map, &imm, mapping_choose, mapping_match);

        // if no edges left delete node
        if (node->edge_map == NULL) {
            graph__delete(r, NULL, NULL);
            *r = NULL;
        }
        else
            break; // is a prefix

        stack__delete(&impl->stack);
    }
}

static
void delete(void **ref, void *info)
{
    graph__delete(ref, info, delete_edge_management);
}


void trie__insert(void **ref, const char *key, void *info)
{
    struct info_impl impl = { NULL, key, 0 };
    struct infostack is = { info, &impl };
    // if there is no root node create it
    if (*ref == NULL)
        graph__insert(ref, &is, sizeof(struct edge_storage),
            insert_edge_management_not_in);
    graph__eloop(ref, &is, match_e, next_insert, insert);
}

void trie__delete(void **ref, const char *key)
{
    struct info_impl impl = { NULL, key, 0 };
    struct infostack is = { NULL, &impl };
    graph__eloop(ref, &is, match_e, next_delete, delete);
}

void trie__access(int qt, void **ref, const char *key, void *info,
    apply_fn apply)
{
    struct info_impl impl = { NULL, key, 0 };
    struct infostack is = { info, &impl };
    switch (qt) {
    case U_QT:
        graph__uloop(ref, &is, match_1, next_u, apply);
        break;
    case E_QT:
        graph__eloop(ref, &is, match_e, next_e, apply);
        break;
    default:
        break;
    }
}


struct info_dump_dot_ext {
    struct info_dump_dot *idd;
    void *parent;
};

static
void dot_edge(void **ref, void *info)
{
    if (*ref != NULL) {
        struct infostack *is = info;
        struct info_dump_dot_ext *idde = is->user;
        FILE *fd = idde->idd->fd;
        void *parent = idde->parent;
        void *node = *ref;
        struct mapping *m = node__data(node, avl__edge_storage());
        fprintf(fd, "n%p->n%p[label=\"%c\"];", parent, m->node, m->byte);
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
    fprintf(fd, "n%p", node);
    if (node->in) {
        fprintf(fd, "[label=\"");
        if (fpd != NULL)
            fpd(fd, ref);
        fprintf(fd, "\"];");
    } else {
        fprintf(fd, "[shape=point];");
    }
    struct info_dump_dot_ext idde = { i, node };
    avl__access(U_QT, &node->edge_map, &idde, NULL, match_1, dot_edge);
}

void trie__dump_dot(FILE *fd, void **ref, void (*fpd)(FILE *, void **))
{
    fprintf(fd, "digraph{rankdir=LR;");
    struct info_dump_dot info = { fd, fpd };
    trie__access(U_QT, ref, NULL, &info, apply_dump_dot);
    fprintf(fd, "}\n");
}
