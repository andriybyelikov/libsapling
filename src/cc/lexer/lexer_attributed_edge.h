#ifndef _LIBSAPLING_LEXER_ATTRIBUTED_EDGE_H_
#define _LIBSAPLING_LEXER_ATTRIBUTED_EDGE_H_

#include "libsapling/dm/avl.h"
#include "libsapling/cc/text.h"

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

    // escape non-printable characters
    char x[2] = { ae->byte, 0 };
    char y[10];
    text__escape(y, x);

    fprintf(stream, "(%p, '%s')", ae->node, y);
}

IMPLEMENT_TYPED_AVL(ae, attributed_edge, attributed_edge__cmp,
    attributed_edge__equ, attributed_edge__fpf)

#endif
