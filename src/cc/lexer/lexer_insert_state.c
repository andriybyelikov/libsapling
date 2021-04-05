#include "libsapling/cc/lexer.h"
#include "cc/lexer/lexer_edge_storage.h"

static
void em_acc_x(node_t *ref, const node_t node, int is_accepting_state)
{
    node->attributed_edges = NULL;
    node->anything_edge = NULL;
    node->is_accepting_state = is_accepting_state;

    *(void **)lexer__data(node) = NULL;

    *ref = node;
}

static
void em_acc_0(node_t *ref, const node_t node, UNUSED void *impl_ptr)
{
    em_acc_x(ref, node, 0);
}

static
void em_acc_1(node_t *ref, const node_t node, UNUSED void *impl_ptr)
{
    em_acc_x(ref, node, 1);
}

void insert_state(node_t *ref, int is_accepting_state)
{
    void *data = NULL;
    struct info_insert info = { sizeof(void *), &data };
    struct info_stack is = { &info, NULL };
    graph__insert(ref, &is, is_accepting_state ? em_acc_1 : em_acc_0,
        sizeof(struct edge_storage));
}
