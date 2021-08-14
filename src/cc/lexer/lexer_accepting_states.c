#include "libsapling/dm/path.h"
#include "libsapling/dm/queue.h"
#include "libsapling/cc/lexer.h"
#include "cc/lexer/lexer_edge_storage.h"
#include "cc/lexer/lexer_attributed_edges.h"
#include "libsapling/dm/typed/typed_common.h"

IMPLEMENT_TYPED_PATH(sp, node_t, NULL, dummy_cmp)
IMPLEMENT_TYPED_QUEUE(sq, node_t, NULL)

typedef node_t *pnode_t;
IMPLEMENT_TYPED_QUEUE(rq, pnode_t, NULL)

static
int match_accepting_states(const node_t *ref,
    UNUSED const struct info_stack *info)
{
    node_t node = *ref;

    return node->is_accepting_state;
}

static
void add_state(node_t *ref, const struct info_stack *info)
{
    node_t accepting_state = *ref;
    node_t *result_set = info->user;

    sq__insert(result_set, accepting_state);
}

static
node_t get_accepting_states(node_t *ref)
{
    node_t result_set = NULL;
    lexer__access(U_QT, ref, &result_set, match_accepting_states, add_state);
    return result_set;
}


struct refs_to_state {
    node_t *result_set;
    node_t accepting_state;
};

static
void get_ae_edges_to_accepting(attributed_edge *data, void *info)
{
    CAST_USER_INFO(struct refs_to_state *, user, info);

    if (data->node == user->accepting_state)
        rq__insert(user->result_set, &data->node);
}

static
void get_edges_to_accepting(node_t *ref, const struct info_stack *info)
{
    node_t node = *ref;
    struct refs_to_state *user = info->user;

    // attributed edge
    ae__access(U_QT, &node->attributed_edges, user, ae__predicate_1,
        get_ae_edges_to_accepting);

    // anything edge
    if (node->anything_edge == user->accepting_state)
        rq__insert(user->result_set, &node->anything_edge);
}

static
void get_accepting_states_refs_apply(node_t *data, void *info)
{
    CAST_USER_INFO(struct refs_to_state *, user, info);
    node_t *result_set = user->result_set;
    node_t initial_state = user->accepting_state; // hacky, should be different struct to pass lexer ref
    node_t accepting_state = *data;

    struct refs_to_state rts = { result_set, accepting_state };
    lexer__access(U_QT, &initial_state, &rts, predicate_1,
        get_edges_to_accepting);
}

node_t lexer__get_accepting_states_refs(node_t *ref)
{
    node_t result_set = NULL;
    node_t accepting_states = get_accepting_states(ref);
    node_t initial_state = *ref;

    struct refs_to_state info = { &result_set, initial_state }; // hacky, should be different struct to pass lexer ref
    sp__access(U_QT, &accepting_states, &info, sp__predicate_1,
        get_accepting_states_refs_apply);

    return result_set;
}


static
void set_data_ptr(node_t *data, void *info)
{
    CAST_USER_INFO(void *, user_data, info);

    void **data_ptr = lexer__data(*data);
    *data_ptr = user_data;
}

void lexer__set_accepting_states_data(node_t *ref, void *data)
{
    node_t accepting_states = get_accepting_states(ref);
    sp__access(U_QT, &accepting_states, data, sp__predicate_1, set_data_ptr);
}
