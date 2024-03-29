#include "libsapling/dm/path.h"
#include "libsapling/cc/lexer.h"
#include "cc/lexer/lexer_edge_storage.h"
#include "cc/lexer/lexer_minimize.h"
#include "cc/lexer/lexer_accepting_states.h"

IMPLEMENT_TYPED_PATH(rp, node_t *, NULL, dummy_cmp)

static
void aux_cat_union(node_t **data, void *info)
{
    CAST_USER_INFO(node_t *, src, info);

    // leftmost is representative
    lexer__union(*data, src);

    node_t node = **data;
    node_t nsrc = *src;
    node->is_accepting_state = nsrc->is_accepting_state;
}

void lexer__concatenation(node_t *dst, node_t *src)
{
    // union of dst accepting states and src initial state
    node_t accepting_states = lexer__get_accepting_states_refs(dst);
    rp__access(U_QT, &accepting_states, src, rp__predicate_1, aux_cat_union);
}
