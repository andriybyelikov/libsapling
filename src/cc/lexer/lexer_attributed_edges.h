#ifndef _LIBSAPLING_LEXER_ATTRIBUTED_EDGES_H_
#define _LIBSAPLING_LEXER_ATTRIBUTED_EDGES_H_

#include "cc/lexer/lexer_attributed_edge.h"

node_t *ae__reserve(node_t *ref, char byte);
attributed_edge *ae__get_edge(node_t *ref, char byte);

#endif
