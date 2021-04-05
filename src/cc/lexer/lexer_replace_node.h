#ifndef LIBSAPLING_LEXER_REPLACE_NODE
#define LIBSAPLING_LEXER_REPLACE_NODE

#include "libsapling/dm/graph.h"

void rea__replace_node_refs_rep(node_t *dst, node_t *ec);

void apply__replace_node_refs_nreps(node_t *data, void *info);

#endif
