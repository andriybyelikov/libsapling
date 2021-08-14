#ifndef _LIBSAPLING_LEXER_EDGE_STORAGE_H_
#define _LIBSAPLING_LEXER_EDGE_STORAGE_H_

#include "libsapling/dm/graph.h"

/*
 * The attributed edges are the edges that go to "another" state on a
 * particular input character. This "other" state can be the origin state,
 * making a loop. The anything edge is followed only when there is no
 * attributed edge that matches the input. Something like this:
 * 
 * if E_QT e in attibuted_edges | attr(e) == input
 *   next = e.target;
 * else
 *   next = anything_edge.target;
 */

struct edge_storage {
    node_t attributed_edges;
    node_t anything_edge;
    int is_accepting_state;
};

#endif
