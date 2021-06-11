#include "libsapling/dm/path.h"
#include "libsapling/cc/lexer.h"
#include "cc/lexer/lexer_edge_storage.h"
#include "cc/lexer/lexer_minimize.h"
#include "cc/lexer/lexer_accepting_states.h"

typedef node_t *pnode_t;
IMPLEMENT_TYPED_PATH(rp, pnode_t, NULL)

static
void aux_cat_union(pnode_t *data, void *info)
{
    struct info_insert *ii = info;
    node_t *src = ii->info;

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
