#include "cc/lexer/lexer_edge_storage.h"
#include "cc/lexer/lexer_insert_state.h"

void lexer__anything(node_t *dst)
{
    insert_state(dst, 0);
    node_t node = *dst;
    insert_state(&node->anything_edge, 1);
}
