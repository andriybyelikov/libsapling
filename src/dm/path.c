#include "libsapling/dm/graph.h"

struct edge_storage {
    node_t next;
};

void *path__data(const node_t node)
{
    return graph__data(node, sizeof(struct edge_storage));
}


/*
 * In the implementation of universally quantified deletion keep in mind when
 * selecting the next node to visit that when a node is deleted the edge that
 * pointed to it is updated to point to the deleted node's successor, therefore
 * it must not be updated again in the next function, otherwise the deleted
 * node's successor would be skipped. The next function is aware of this event
 * through the implementation information passed to it in the info_impl struct.
 */

struct info_impl {
    int deletion_happened;
};

static
int next(node_t **ref, const struct info_stack *info)
{
    node_t node = **ref;
    struct info_impl *impl = info->impl;

    if (impl->deletion_happened)
        impl->deletion_happened = 0;
    else
        *ref = &node->next;

    return 1;
}

void path__access(enum qt qt, node_t *ref, void *info, predicate_t predicate,
    apply_t apply)
{
    struct info_impl impl = { 0 };
    struct info_stack is = { info, &impl };
    graph__access(qt, ref, &is, predicate, apply, next);
}


static
void ins_em(node_t *ref, const node_t node, UNUSED void *impl_ptr)
{
    node->next = *ref;
    *ref = node;
}

static
void insert(node_t *ref, const struct info_stack *info)
{
    graph__insert(ref, info, ins_em, sizeof(struct edge_storage));
}

void path__insert(node_t *ref, void *info, predicate_t predicate)
{
    path__access(E_QT, ref, info, predicate, insert);
}


static
void del_em(node_t *ref, const node_t node, void *impl_ptr)
{
    struct info_impl *impl = impl_ptr;

    *ref = node->next;
    impl->deletion_happened = 1;
}

static
void delete(node_t *ref, const struct info_stack *info)
{
    graph__delete(ref, info, del_em);
}

void path__delete(enum qt qt, node_t *ref, void *info, predicate_t predicate)
{
    path__access(qt, ref, info, predicate, delete);
}


static
void all_access_adapter(node_t *ref, void *info, apply_t apply)
{
    path__access(U_QT, ref, info, predicate_1, apply);
}


static
void print_data_aux0(FILE *stream, const node_t node, fpfdata_fn fpfdata, void *impl)
{
    fpfdata(stream, path__data(node));
}

void path__print_data(FILE *stream, node_t *ref, fpfdata_fn fpfdata)
{
    graph__print_data(stream, ref, all_access_adapter, print_data_aux0, fpfdata);
}


static
void dump_dot_aux0(FILE *stream, const node_t node, fpfdata_fn fpfdata, void *impl)
{
    fprintf(stream, "n%p[label=\"", node);
    if (fpfdata != NULL)
        fpfdata(stream, path__data(node));
    fprintf(stream, "\"];");

    if (node->next != NULL)
        fprintf(stream, "n%p->n%p;", node, node->next);
}

void path__dump_dot(FILE *stream, node_t *ref, fpfdata_fn fpfdata)
{
    graph__dump_dot(stream, ref, all_access_adapter, dump_dot_aux0, fpfdata, "");
}

int path__length(node_t *ref)
{
    return graph__length(ref, all_access_adapter);
}


struct position {
    int pos;
    node_t *ref;
};

static
int pos_predicate(UNUSED const node_t *ref, const struct info_stack *info)
{
    struct position *user = info->user;

    return user->pos-- == 0;
}

static
void pos_apply(node_t *ref, const struct info_stack *info)
{
    struct position *user = info->user;

    user->ref = ref;
}

node_t *path__position(node_t *ref, int pos)
{
    struct position info = { pos, NULL };
    path__access(E_QT, ref, &info, pos_predicate, pos_apply);
    return info.ref;
}
