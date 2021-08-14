#ifndef _LIBSAPLING_LEXER_ACCEPTING_STATES_H_
#define _LIBSAPLING_LEXER_ACCEPTING_STATES_H_

#include "libsapling/dm/graph.h"

node_t lexer__get_accepting_states_refs(node_t *ref);
void lexer__set_accepting_states_data(node_t *ref, void *data);

#endif
