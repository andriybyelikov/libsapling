#ifndef LIBSAPLING_LEXER_ATTRIBUTED_EDGES
#define LIBSAPLING_LEXER_ATTRIBUTED_EDGES

#include "cc/lexer/lexer_attributed_edge.h"

node_t *ae__reserve(node_t *ref, char byte);
attributed_edge *ae__get_edge(node_t *ref, char byte);

#endif
