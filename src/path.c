#include "graph.h"

struct edge_storage {
    void *next;
};

size_t path__edge_storage(void)
{
    return sizeof(struct edge_storage);
}

void *path__node__data(void **ref)
{
    return node__data(*ref, path__edge_storage());
}


struct info_impl {
    int deletion_happened;
};

static
void next(void ***ref, void *info)
{
    struct info_impl *i = get_impl_info(info);
    if (i->deletion_happened) {
        i->deletion_happened = 0;
    } else {
        struct edge_storage *node = **ref;
        *ref = &node->next;
    }
}


static
void insert_edge_management(void **ref, struct edge_storage *node, void *info)
{
    node->next = *ref;
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
    *ref = node->next;
    struct info_impl *i = get_impl_info(info);
    i->deletion_happened = 1;
}

static
void delete(void **ref, void *info)
{
    graph__delete(ref, info, delete_edge_management);
}


void path__insert(void **ref, void *info, match_fn match)
{
    struct info_impl impl = { sizeof(struct edge_storage) };
    struct infostack is = { info, &impl };
    graph__eloop(ref, &is, match, next, insert);
}

void path__delete(int qt, void **ref, void *info, match_fn match)
{
    struct info_impl impl = { 0 };
    struct infostack is = { info, &impl };
    switch (qt) {
    case U_QT:
        graph__uloop(ref, &is, match, next, delete);
        break;
    case E_QT:
        graph__eloop(ref, &is, match, next, delete);
        break;
    default:
        break;
    }
}

void path__access(int qt, void **ref, void *info, match_fn match,
    apply_fn apply)
{
    struct info_impl impl = { 0 };
    struct infostack is = { info, &impl };
    switch (qt) {
    case U_QT:
        graph__uloop(ref, &is, match, next, apply);
        break;
    case E_QT:
        graph__eloop(ref, &is, match, next, apply);
        break;
    default:
        break;
    }
}


static
void apply_dump_dot(void **ref, void *info)
{
    struct edge_storage *node = *ref;
    struct info_dump_dot *i = get_user_info(info);
    FILE *fd = i->fd;
    void (*fpd)(FILE *, void **) = i->fpd;
    fprintf(fd, "n%p[label=\"", node);
    if (fpd != NULL)
        fpd(fd, ref);
    fprintf(fd, "\"];");
    if (node->next != NULL)
        fprintf(fd, "n%p->n%p;", node, node->next);
}

void path__dump_dot(FILE *fd, void **ref, void (*fpd)(FILE *fd, void **ref))
{
    fprintf(fd, "digraph{");
    struct info_dump_dot info = { fd, fpd };
    path__access(U_QT, ref, &info, match_1, apply_dump_dot);
    fprintf(fd, "}\n");
}
