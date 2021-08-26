#include "libsapling/dm/path.h"
#include "libsapling/dm/queue.h"
#include "libsapling/cc/lexer.h"
#include "cc/lexer/lexer_edge_storage.h"
#include "cc/lexer/lexer_attributed_edges.h"
#include "cc/lexer/lexer_ec_def.h"
#include "cc/lexer/lexer_replace_node.h"

IMPLEMENT_TYPED_PATH(sp, node_t, NULL, dummy_cmp)
IMPLEMENT_TYPED_QUEUE(sq, node_t, NULL)


struct info_build_ec {
    node_t repr;
    node_t *ec;
};

static
int match__eq_state(const node_t *data, void *info)
{
    CAST_USER_INFO(struct info_build_ec *, user, info);

    return state__equ(&user->repr, data);
}

static
void build_ec(node_t *data, void *info)
{
    CAST_USER_INFO(struct info_build_ec *, user, info);

    if (*user->ec == NULL)
        ec__insert(user->ec, user->repr);
    ec__insert(user->ec, *data);
}


static
int match__state_in_ec(const node_t *data, void *info)
{
    CAST_USER_INFO(node_t *, ec, info);

    return ec__in(ec, *data);
}

static
void apply__get_states(node_t *ref, const struct info_stack *info)
{
    node_t state = *ref;
    node_t *list = info->user;

    sq__insert(list, state);
}


// Minimize debug diagnostics
///
static
void pretty_print_states(node_t *data, void *info)
{
    CAST_USER_INFO(node_t *, en, info);

    fprintf(stderr, "%s", lexer__to_string(data, en));
}
///


/* Equivalent of arranging a nodes x nodes tables and traversing one of the
 * triangles excluding the diagonal, a node is selected to be compared with
 * the rest and an equivalence class is built if the graph has another node
 * that is equivalent to the selected node, the selected node is the
 * representative and the rest have their references replaced by it in the
 * graph, the non-representatives are discarded and the representative is
 * added to the back of the list of nodes, since I thought that it could
 * be equal to another state later when other states are merged, but I think
 * this is not the case, since the merging does not change the node's
 * discriminant information -- Andriy Byelikov
 */

void lexer__minimize(node_t *dst)
{
    node_t en = lexer__enumerate_states(dst);

    int changes = 1;
    while (changes) {
        changes = 0;

        // initialize list of states
        node_t states = NULL;
        lexer__access(U_QT, dst, &states, predicate_1, apply__get_states);

        // Minimize debug diagnostics
        //fprintf(stderr, "---n");
        //sp__access(U_QT, &states, &en, sp__predicate_1, pretty_print_states);

        // list of built equivalence classes
        node_t ecq = NULL;
        while (states != NULL) {
            // get a state that is not yet in any equivalence class
            node_t state = sq__delete(&states);

            // build all equivalence classes for this state and push them to
            // ecq
            node_t ec = NULL;
            struct info_build_ec bec = { state, &ec };
            sp__access(U_QT, &states, &bec, match__eq_state, build_ec);

            if (ec != NULL) {
                ecq__insert(&ecq, ec);
                // remove all nodes of class from queue
                sp__delete(U_QT, &states, &ec, match__state_in_ec);
            }
        }

        // do replacements where necessary
        if (ecq != NULL) {
            changes = 1;

            while (ecq != NULL) {
                node_t ec = ecq__delete(&ecq);

                // Minimize debug diagnostics
                //fprintf(stderr, "---equiv----\n");
                //sp__access(U_QT, &ec, &en, sp__predicate_1,
                //    pretty_print_states);

                // initialize static list of states
                node_t static_states = NULL;
                lexer__access(U_QT, dst, &static_states, predicate_1,
                    apply__get_states);

                // in whole graph replace class nodes with representative
                sp__access(U_QT, &static_states, &ec, sp__predicate_1,
                    apply__replace_node_refs_nreps);

                // update outer reference if necessary
                if (ec__in(&ec, *dst))
                    *dst = ec__access_representative(&ec);

                // TODO free non-representative nodes
            }
        }
    }
}
