#include "libsapling/dm/path.h"
#include "cc/lexer/lexer_edge_storage.h"
#include "cc/lexer/lexer_attributed_edges.h"
#include "cc/lexer/lexer_ec_def.h"
#include "cc/lexer/lexer_replace_node.h"
#include "cc/lexer/lexer_minimize.h"
#include "libsapling/dm/typed/typed_common.h"

IMPLEMENT_TYPED_PATH(sp, node_t, NULL, dummy_cmp)

static
void rea__union_foreach_edge(attributed_edge *data, void *info)
{
    CAST_USER_INFO(struct representative_and_equivalence_class_queue *, user,
        info);

    node_t repr = user->representative;
    node_t *queue = user->equivalence_class_queue;

    if (!ae__in(&repr->attributed_edges,
        (attributed_edge){ .byte = data->byte })) {
        ae__insert(&repr->attributed_edges, *data, ae__cmp_predicate);
        // TODO delete unused edge
    } else {
        if (ae__get_edge(&repr->attributed_edges, data->byte)->node != data->node) {
            node_t new_ec = NULL;
            ec__insert(&new_ec, ae__get_edge(&repr->attributed_edges,
                data->byte)->node);
            ec__insert(&new_ec, data->node);
            ecq__insert(queue, new_ec);
        }
    }
}

static
void apply__move_edges_to_repr(node_t *data, void *info)
{
    CAST_USER_INFO(struct representative_and_equivalence_class_queue *, user,
        info);

    node_t repr = user->representative;
    node_t *queue = user->equivalence_class_queue;

    if (data != NULL) { // APAÑO
        node_t node = *data;

        // attributed edges
        ae__access(U_QT, &node->attributed_edges, user, ae__predicate_1,
            rea__union_foreach_edge);

        // anything edge
        if (repr->anything_edge == NULL) {
            repr->anything_edge = node->anything_edge;
        } else {
            if (node->anything_edge != NULL && repr->anything_edge != node->anything_edge) {
                node_t new_ec = NULL;
                ec__insert(&new_ec, repr->anything_edge);
                ec__insert(&new_ec, node->anything_edge);
                ecq__insert(queue, new_ec);
            }
        }

        // union of is_accepting_state
        repr->is_accepting_state = repr->is_accepting_state ||
            node->is_accepting_state;
    }
}

/*
 * Merge the initial states of both graphs into a single state. This state
 * becomes the new initial state. Successor states that are reached on the same
 * input and their respective edges are also merged in propagation.
 */

void lexer__union(node_t *dst, node_t *src)
{
    node_t ec_queue = NULL;

    node_t initial_ec = NULL;
    ec__insert(&initial_ec, *dst);
    ec__insert(&initial_ec, *src);
    ecq__insert(&ec_queue, initial_ec);

    while (ec_queue != NULL) {
        node_t ec = ecq__access(&ec_queue);

        // copy non-representative nodes' edges over to representative node
        struct representative_and_equivalence_class_queue info = {
            ec__access_representative(&ec),
            &ec_queue
        };
        ec__access_non_representatives(U_QT, &ec, &info, ec__predicate_1,
            apply__move_edges_to_repr);

        // starting from each state in the equivalence class replace
        // non-representatives' adresses with representative's address

        // replace starting from representative and update external reference
        rea__replace_node_refs_rep(dst, &ec);

        // replace starting from non-representatives
        sp__access(U_QT, &ec, &ec, sp__predicate_1, apply__replace_node_refs_nreps);

        ecq__delete(&ec_queue);
    }
}
