#include <string.h>
#include "cc/lexer/lexer_edge_storage.h"
#include "cc/lexer/lexer_attributed_edges.h"
#include "cc/lexer/lexer_insert_state.h"

void lexer__literal(node_t *dst, const char *expr)
{
    for (int i = 1; i < strlen(expr) - 1; i++) { // trim quotation marks
        insert_state(dst, 0);
        node_t node = *dst;
        dst = ae__reserve(&node->attributed_edges, expr[i]);
    }
    insert_state(dst, 1);
}
