#ifndef LIBSAPLING_LEXER_ACCEPTING_STATES
#define LIBSAPLING_LEXER_ACCEPTING_STATES

#include "libsapling/dm/graph.h"

node_t lexer__get_accepting_states_refs(node_t *ref);
void lexer__set_accepting_states_data(node_t *ref, void *data);

#endif
