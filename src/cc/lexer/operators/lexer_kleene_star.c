#include "libsapling/dm/path.h"
#include "libsapling/cc/lexer.h"
#include "cc/lexer/lexer_edge_storage.h"
#include "cc/lexer/lexer_minimize.h"
#include "cc/lexer/lexer_accepting_states.h"

IMPLEMENT_TYPED_PATH(rp, pnode_t, NULL, dummy_cmp)

static
void aux_ks_union(pnode_t *data, void *info)
{
    CAST_USER_INFO(node_t *, src, info);

    lexer__union(src, *data);

    node_t node = **data;
    node->is_accepting_state = 1;
}

void lexer__kleene_star(node_t *dst)
{
    // union of dst accepting states and dst initial state
    node_t accepting_states = lexer__get_accepting_states_refs(dst);
    rp__access(U_QT, &accepting_states, dst, rp__predicate_1, aux_ks_union);
}
