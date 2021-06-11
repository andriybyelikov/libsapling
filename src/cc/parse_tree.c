#include <string.h>
#include <stdlib.h>
#include "libsapling/dm/path.h"
#include "libsapling/dm/queue.h"

struct edge_storage {
    node_t children; // queue
};

const char *parse_tree__data(node_t node)
{
    return (const char *)graph__data(node, sizeof(struct edge_storage));
}


static
void ins_em(node_t *ref, const node_t node, void *impl)
{
    node->children = NULL;
    *ref = node;
}

node_t parse_tree__create_node(char *string)
{
    node_t node = NULL;

    const size_t string_storage = strlen(string) + 1;
    {
        struct info_insert info2 = { string_storage, string };
        struct info_stack is = { &info2, NULL };
        graph__insert(&node, &is, ins_em, sizeof(struct edge_storage));
    }

    return node;
}


IMPLEMENT_TYPED_QUEUE(children, node_t, NULL)

void parse_tree__append_child(node_t *ref, node_t child)
{
    node_t node = *ref;
    children__insert(&node->children, child);
}


node_t parse_tree__get_child_by_position(node_t *ref, int pos)
{
    node_t node = *ref;
    return children__access(path__position(&node->children, pos));
}


struct get_child_by_str {
    node_t child;
    const char *str;
};

static
int is_str(const node_t *ref, const struct info_stack *info)
{
    node_t node = *ref;
    struct get_child_by_str *user = info->user;

    node_t child = *(node_t *)path__data(node);
    return !strcmp(user->str, parse_tree__data(child));
}

static
void get_child(node_t *ref, const struct info_stack *info)
{
    node_t node = *ref;
    struct get_child_by_str *user = info->user;

    if (node != NULL)
        user->child = children__access(ref);
}

node_t parse_tree__get_child_by_string(node_t *ref, const char *str)
{
    node_t node = *ref;

    struct get_child_by_str info = { NULL, str };
    path__access(E_QT, &node->children, &info, is_str, get_child);
    return info.child;
}


struct info_impl {
    node_t queue;
};

typedef node_t *pnode_t;
IMPLEMENT_TYPED_QUEUE(trail, pnode_t, NULL)

static
void expand_children(node_t *ref, const struct info_stack *info)
{
    node_t node = *ref;
    struct info_impl *user = info->user;

    if (node != NULL)
        trail__insert(&user->queue, path__data(node));
}

static
int next_u(node_t **ref, const struct info_stack *info)
{
    node_t node = **ref;
    struct info_impl *impl = info->impl;

    path__access(U_QT, &node->children, impl, predicate_1, expand_children);
    if (impl->queue == NULL)
        *ref = NULL;
    else
        *ref = trail__delete(&impl->queue);
    return *ref != NULL;
}

static
void parse_tree__access(node_t *ref, void *info, apply_t apply)
{
    struct info_impl impl = { NULL };
    struct info_stack is = { info, &impl };
    graph__access(U_QT, ref, &is, predicate_1, apply, next_u);
}

static
void all_access_adapter(node_t *ref, void *info, apply_t apply)
{
    parse_tree__access(ref, info, apply);
}


struct dump_dot_aux {
    FILE *stream;
    node_t parent;
};

static
void dump_dot_aux1(node_t *ref, const struct info_stack *info)
{
    node_t node = *ref;
    struct dump_dot_aux *user = info->user;

    FILE *stream = user->stream;
    node_t parent = user->parent;
    node_t child = *(node_t *)path__data(node);
    fprintf(stream, "n%p->n%p;", parent, child);
}

static
void dump_dot_aux0(FILE *stream, const node_t node, fpfdata_fn fpfdata, void *impl)
{
    fprintf(stream, "n%p[label=\"", node);
    if (fpfdata != NULL)
        fpfdata(stream, parse_tree__data(node));
    fprintf(stream, "\"];");

    struct dump_dot_aux info = { stream, node };
    path__access(U_QT, &node->children, &info, predicate_1, dump_dot_aux1);
}

static
void fpfdata_str(FILE *stream, const void *data)
{
    fprintf(stream, "%s", (const char *)data);
}

void parse_tree__dump_dot(FILE *stream, node_t *ref)
{
    graph__dump_dot(stream, ref, all_access_adapter, dump_dot_aux0,
        fpfdata_str, "");
}
