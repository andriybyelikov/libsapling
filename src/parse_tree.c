#include "graph.h"
#include <string.h>
#include <stdlib.h>
#include "path.h"
#include "queue.h"

struct edge_storage {
    void *children;
};

static
size_t parse_tree__edge_storage(void)
{
    return sizeof(struct edge_storage);
}

static
void *parse_tree__node__data(void **ref)
{
    return node__data(*ref, parse_tree__edge_storage());
}


static
void em_init(void **ref, struct edge_storage *node, void *info)
{
    node->children = NULL;
    *ref = node;
}

void *parse_tree__create_node(const char *string)
{
    void *node = NULL;

    const size_t string_storage = strlen(string) + 1;
    struct data *info = malloc(sizeof(size_t) + string_storage);
    *(size_t *)info = string_storage;
    char *dst = (char *)info + sizeof(size_t);
    strcpy(dst, string);
    {
        struct infostack is = { info, NULL };
        graph__insert(&node, &is, sizeof(struct edge_storage), em_init);
    }
    free(info);

    return node;
}

struct child {
    struct edge_storage *child;
};

struct insert_child {
    size_t size;
    struct child data;
};

void parse_tree__append_child(void **ref, void *child)
{
    struct edge_storage *node = *ref;

    struct insert_child info = {
        sizeof(struct child),
        { (struct edge_storage *)child }
    };
    queue__insert(&node->children, &info);
}


const char *parse_tree__node__get_string(void **ref)
{
    return parse_tree__node__data(ref);
}


struct by_string {
    const char *string;
    void *node;
};

static
int match_by_string(void **ref, void *info)
{
    struct by_string *user = get_user_info(info);
    struct edge_storage *node = *(void **)path__node__data(ref);
    return !strcmp(parse_tree__node__get_string((void**)&node), user->string);
}

static
void apply_by_string(void **ref, void *info)
{
    if (*ref != NULL) {
        struct by_string *user = get_user_info(info);
        user->node = *(void **)path__node__data(ref);
    }
}

void *parse_tree__get_child_by_string(void **ref, const char *string)
{
    struct by_string info = { string, NULL };
    struct edge_storage *node = *ref;
    path__access(E_QT, &node->children, &info, match_by_string, apply_by_string);
    return info.node;
}


struct by_position {
    int pos;
    void *node;
};

static
int match_by_position(void **ref, void *info)
{
    struct by_position *user = get_user_info(info);
    return user->pos-- == 0;
}

static
void apply_by_position(void **ref, void *info)
{
    if (*ref != NULL) {
        struct by_position *user = get_user_info(info);
        user->node = *(void **)path__node__data(ref);
    }
}

void *parse_tree__get_child_by_position(void **ref, int pos)
{
    struct by_position info = { pos, NULL };
    struct edge_storage *node = *ref;
    path__access(E_QT, &node->children, &info, match_by_position,
        apply_by_position);
    return info.node;
}

// -----------------------------------------------------------------------------

struct info_impl {
    void *queue;
};

struct ref {
    void **ref;
};

struct insert_ref {
    size_t size;
    struct ref data;
};

static
void expand_children(void **ref, void *info)
{
    if (*ref != NULL) {
        struct info_impl *impl = get_user_info(info);
        struct insert_ref ir = {
            sizeof(void **),
            { parse_tree__node__data(ref) }
        };
        queue__insert(&impl->queue, &ir);
    }
}

static
void next_u(void ***ref, void *info)
{
    struct info_impl *impl = get_impl_info(info);
    struct edge_storage *node = **ref;
    path__access(U_QT, &node->children, impl, match_1, expand_children);
    if (impl->queue == NULL) {
        *ref = &impl->queue;
    } else {
        *ref = *(void **)queue__access(&impl->queue);
        queue__delete(&impl->queue);
    }
}

static
void parse_tree__access(void **ref, void *info, apply_fn apply)
{
    struct info_impl impl = { NULL };
    struct infostack is = { info, &impl };
    graph__uloop(ref, &is, match_1, next_u, apply);
}


struct info_dump_dot_ext {
    struct info_dump_dot *idd;
    void *parent;
};

static
void dump_children(void **ref, void *info)
{
    struct info_dump_dot_ext *i = get_user_info(info);
    FILE *fd = i->idd->fd;
    fprintf(fd, "n%p->n%p;", i->parent, *(void **)parse_tree__node__data(ref));
}

static
void apply_dump_dot(void **ref, void *info)
{
    struct edge_storage *node = *ref;
    struct info_dump_dot *i = get_user_info(info);
    FILE *fd = i->fd;

    fprintf(fd, "n%p[label=\"%s\"];", node, parse_tree__node__data(ref));

    struct info_dump_dot_ext idde = { i, node };
    path__access(U_QT, &node->children, &idde, match_1, dump_children);
}

void parse_tree__dump_dot(FILE *fd, void **ref)
{
    fprintf(fd, "digraph{");
    struct info_dump_dot info = { fd, NULL };
    parse_tree__access(ref, &info, apply_dump_dot);
    fprintf(fd, "}\n");
}
