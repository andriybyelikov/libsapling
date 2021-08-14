#include "cc/lexer/lexer_edge_storage.h"
#include "cc/lexer/lexer_attributed_edges.h"
#include "cc/lexer/lexer_ec_def.h"

static
void apply__replace_attributed_edges(attributed_edge *data, void *info)
{
    CAST_USER_INFO(node_t *, ec, info);

    // technically no need to check for NULL here since it wouldn't be in
    if (ec__in(ec, data->node))
        data->node = ec__access_representative(ec);
}

static
void apply__replace_edges(node_t *ref, const struct info_stack *info)
{
    node_t node = *ref;
    node_t *ec = info->user;

    // replace refs in attributed edges
    ae__access(U_QT, &node->attributed_edges, ec, ae__predicate_1,
        apply__replace_attributed_edges);

    // replace ref in anything edge
    if (node->anything_edge != NULL && ec__in(ec, node->anything_edge))
        node->anything_edge = ec__access_representative(ec);
}

void rea__replace_node_refs_rep(node_t *dst, node_t *ec)
{
    node_t repr = ec__access_representative(ec);

    // treat external reference
    if (ec__in(ec, *dst))
        *dst = repr;

    lexer__access(U_QT, &repr, &ec, predicate_1, apply__replace_edges);
}

void apply__replace_node_refs_nreps(node_t *data, void *info)
{
    CAST_USER_INFO(node_t *, user, info);

    lexer__access(U_QT, data, user, predicate_1, apply__replace_edges);
}
