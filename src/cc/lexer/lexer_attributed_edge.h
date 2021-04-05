#ifndef LIBSAPLING_ATTRIBUTED_EDGE
#define LIBSAPLING_ATTRIBUTED_EDGE

#include "libsapling/dm/avl.h"

typedef struct {
    node_t node;
    char byte;
} attributed_edge;

static
int attributed_edge__cmp(const void *a, const void *b)
{
    return ((attributed_edge *)a)->byte <= ((attributed_edge *)b)->byte;
}

static
int attributed_edge__equ(const void *a, const void *b)
{
    return ((attributed_edge *)a)->byte == ((attributed_edge *)b)->byte;
}

static
void attributed_edge__fpf(FILE *stream, const void *data)
{
    attributed_edge *ae = (attributed_edge *)data;
    fprintf(stream, "(%p, '%c')", ae->node, ae->byte);
}

IMPLEMENT_TYPED_AVL(ae, attributed_edge, attributed_edge__cmp,
    attributed_edge__equ, attributed_edge__fpf)

#endif
