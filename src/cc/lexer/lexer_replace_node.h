#ifndef _LIBSAPLING_LEXER_REPLACE_NODE_H_
#define _LIBSAPLING_LEXER_REPLACE_NODE_H_

#include "libsapling/dm/graph.h"

void rea__replace_node_refs_rep(node_t *dst, node_t *ec);

void apply__replace_node_refs_nreps(node_t *data, void *info);

#endif
