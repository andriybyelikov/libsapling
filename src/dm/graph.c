#include <stdlib.h>
#include <string.h>
#include "libsapling/dm/graph.h"

int predicate_0(UNUSED const node_t *ref, UNUSED const struct info_stack *info)
{
    return 0;
}

int predicate_1(UNUSED const node_t *ref, UNUSED const struct info_stack *info)
{
    return 1;
}

void *graph__data(const node_t node, size_t edge_storage)
{
    return (char *)node + edge_storage;
}

void graph__insert(node_t *ref, const struct info_stack *info,
    edge_management_t edge_management, size_t edge_storage)
{
    struct info_insert *user = info->user;

    node_t node = malloc(edge_storage + user->size);
    memcpy(graph__data(node, edge_storage), user->data, user->size);
    edge_management(ref, node, info->impl);
}

void graph__delete(node_t *ref, const struct info_stack *info,
    edge_management_t edge_management)
{
    node_t node = *ref;

    if (node != NULL) {
        edge_management(ref, node, info->impl);
        free(node);
    }
}


static
int u_cond(node_t **ref, const struct info_stack *info, next_t next)
{
    if (**ref == NULL)
        return 0;
    else
        return next(ref, info);
}

static
void u_loop(node_t *ref, const struct info_stack *info, predicate_t predicate,
    apply_t apply, next_t next)
{
    do {
        if (*ref != NULL && predicate(ref, info))
            apply(ref, info);
    } while (u_cond(&ref, info, next));
}


static
int e_cond(node_t **ref, const struct info_stack *info, next_t next,
    predicate_t predicate)
{
    if (**ref == NULL || predicate(*ref, info))
        return 0;
    else
        return next(ref, info);
}

static
void e_loop(node_t *ref, const struct info_stack *info, predicate_t predicate,
    apply_t apply, next_t next)
{
    while (e_cond(&ref, info, next, predicate))
        ;
    apply(ref, info);
}


void graph__access(enum qt qt, node_t *ref, const struct info_stack *info,
    predicate_t predicate, apply_t apply, next_t next)
{
    if (qt == U_QT)
        u_loop(ref, info, predicate, apply, next);
    else // qt == E_QT
        e_loop(ref, info, predicate, apply, next);
}


struct info_print {
    FILE *stream;
    fpfnode_t fpfnode;
    fpfdata_t fpfdata;
    int first;
};

static
void graph__print_aux0(node_t *ref, const struct info_stack *info)
{
    const node_t node = *ref;
    struct info_print *user = info->user;
    if (!user->first)
        fprintf(user->stream, ", ");
    else
        user->first = 0;
    user->fpfnode(user->stream, node, user->fpfdata, info->impl);
}

void graph__print(FILE *stream, node_t *ref,
    all_access_adapter_fn access_adapter, fpfnode_t fpfnode,
    fpfdata_t fpfdata)
{
    if (*ref == NULL) {
        fprintf(stream, "{}");
        return;
    }

    fprintf(stream, "{ ");
    struct info_print info = { stream, fpfnode, fpfdata, 1 };
    access_adapter(ref, &info, graph__print_aux0);
    fprintf(stream, " }");
}


void graph__println(FILE *stream, node_t *ref,
    all_access_adapter_fn access_adapter, fpfnode_t fpfnode,
    fpfdata_t fpfdata)
{
    graph__print(stream, ref, access_adapter, fpfnode, fpfdata);
    fprintf(stream, "\n");
}


static
void graph__dump_dot_aux0(node_t *ref, const struct info_stack *info)
{
    const node_t node = *ref;
    struct info_print *user = info->user;
    user->fpfnode(user->stream, node, user->fpfdata, info->impl);
}

void graph__dump_dot(FILE *stream, node_t *ref,
    all_access_adapter_fn access_adapter, fpfnode_t fpfnode,
    fpfdata_t fpfdata, const char *graph_attributes)
{
    fprintf(stream, "digraph{%sref[label=\"&\";shape=square];",
        graph_attributes);
    if (*ref != NULL)
        fprintf(stream, "ref->n%p;", *ref);
    struct info_print info = { stream, fpfnode, fpfdata };
    access_adapter(ref, &info, graph__dump_dot_aux0);
    fprintf(stream, "}\n");
}


struct length {
    int len;
};

static
void len_apply(UNUSED node_t *ref, const struct info_stack *info)
{
    struct length *user = info->user;

    user->len++;
}

int graph__length(const node_t *ref, all_access_adapter_fn access_adapter)
{
    struct length info = { 0 };
    access_adapter(ref, &info, len_apply);
    return info.len;
}
