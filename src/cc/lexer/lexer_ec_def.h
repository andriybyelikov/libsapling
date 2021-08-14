#ifndef LIBSAPLING_LEXER_EC_DEF
#define LIBSAPLING_LEXER_EC_DEF

#include "libsapling/dm/equivalence_class.h"
#include "libsapling/dm/queue.h"
#include "libsapling/cc/lexer.h"

struct ae_equal {
    node_t s1;
    int all_eq;
};

static
int match__ae_neq(const attributed_edge *data, void *info)
{
    CAST_USER_INFO(struct ae_equal *, user, info);
    node_t node = user->s1;
    const attributed_edge *ae0 = data;
    attributed_edge *ae1 = NULL;

    if (ae__in(&node->attributed_edges, (attributed_edge){ .byte = ae0->byte }))
        ae1 = ae__get_edge(&node->attributed_edges, ae0->byte);

    if (ae1 == NULL || ae0->node != ae1->node)
        return 1;
    return 0;
}

static
void apply__ae_nall_eq(UNUSED attributed_edge *data, void *info)
{
    CAST_USER_INFO(struct ae_equal *, user, info);

    user->all_eq = 0;
}

static
int state__equ(const void *a, const void *b)
{
    node_t s0 = *(node_t *)a;
    node_t s1 = *(node_t *)b;

    // comparisons ordered by computational cost

    if (s0->is_accepting_state != s1->is_accepting_state)
        return 0;

    if (s0->anything_edge != s1->anything_edge)
        return 0;

    // compare data pointer
    void **d0 = lexer__data(s0);
    void **d1 = lexer__data(s1);
    if (*d0 != *d1)
        return 0;

    // compare attributed edges

    // compare NULL case
    if (s0->attributed_edges == NULL || s1->attributed_edges == NULL)
        if (s0->attributed_edges != s1->attributed_edges)
            return 0;

    struct ae_equal info0 = { s1, 1 };
    ae__access(U_QT, &s0->attributed_edges, &info0,
        match__ae_neq, apply__ae_nall_eq);

    struct ae_equal info1 = { s0, 1 };
    ae__access(U_QT, &s1->attributed_edges, &info1,
        match__ae_neq, apply__ae_nall_eq);

    return info0.all_eq && info1.all_eq;
}


static
void ec_fpf(FILE *stream, const void *data)
{
    node_t *ae = (node_t *)data;
    fprintf(stream, "%p", ae);
}


IMPLEMENT_TYPED_EQUIVALENCE_CLASS(ec, node_t, ec_fpf, state__equ)
IMPLEMENT_TYPED_QUEUE(ecq, node_t, NULL)

struct representative_and_equivalence_class_queue {
    node_t representative;
    node_t *equivalence_class_queue;
};

#endif
