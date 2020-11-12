#include <stdlib.h> // malloc, free
#include <string.h> // memcpy
#include "graph.h"

int match_0(void **ref, void *info)
{
    return 0;
}

int match_1(void **ref, void *info)
{
    return 1;
}


static
int universal_decision(void ***ref, void *info, next_fn next)
{
    if (**ref == NULL) {
        return 0;
    } else {
        next(ref, info);
        return 1;
    }
}

void graph__uloop(void **ref, void *info,
    match_fn match, next_fn next, apply_fn apply)
{
    do {
        if (*ref != NULL && match(ref, info))
            if (apply != NULL)
                apply(ref, info);
    } while (universal_decision(&ref, info, next));
}


static
int existential_decision(void ***ref, void *info, match_fn match, next_fn next)
{
    if (**ref == NULL || match(*ref, info)) {
        return 0;
    } else {
        next(ref, info);
        return 1;
    }
}

void graph__eloop(void **ref, void *info,
    match_fn match, next_fn next, apply_fn apply)
{
    while (existential_decision(&ref, info, match, next))
        ;
    if (apply != NULL)
        apply(ref, info);
}


static
void *node__allocate(size_t edge_storage, size_t data_storage)
{
    return malloc(edge_storage + data_storage);
}

void graph__insert(void **ref, void *info, size_t edge_storage,
    edge_management_fn edge_management)
{
    struct infostack *is = info;
    void *user = is->user;
    const size_t data_storage = *(size_t *)user;
    const void *data = (char *)user + sizeof(size_t);
    struct edge_storage *node = node__allocate(edge_storage, data_storage);
    memcpy(node__data(node, edge_storage), data, data_storage);
    edge_management(ref, node, info);
}


static
void node__deallocate(void *ref)
{
    free(ref);
}

void graph__delete(void **ref, void *info,
    edge_management_fn edge_management)
{
    if (*ref != NULL) {
        struct edge_storage *node = *ref;
        if (edge_management != NULL)
            edge_management(ref, node, info);
        node__deallocate(node);
    }
}


void *node__data(void *ref, size_t edge_storage)
{
    return (char *)ref + edge_storage;
}
