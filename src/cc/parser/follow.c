#include <stdlib.h>
#include <libsapling/cc/production.h>
#include "libsapling/cc/parser/grammar.h"
#include "libsapling/cc/parser/first.h"
#include "libsapling/cc/parser/aux/symbol_set.h"
#include "libsapling/cc/parser/aux/production_set.h"

node_t *compute_follow_set(grammar_t g, int nonterminal);

struct info_compute_follow_set {
    node_t *follow_set;
    int nonterminal;
    grammar_t g;
};

static
void compute_follow_set_apply(production_t *data, void *info)
{
    CAST_USER_INFO(struct info_compute_follow_set *, user, info);

    for (int i = 0; i < production__len(*data); i++) {
        int sym = *production__get(*data, i);
        if (sym != user->nonterminal)
            continue;
        if (i == production__len(*data) - 1) {
            // FOLLOW(B) = FOLLOW(B) ∪ FOLLOW(A), where B = nonterminal
            // compute FOLLOW(A):
            node_t *follow_set = compute_follow_set(user->g,
                production__id(*data));
            // FOLLOW(B) = FOLLOW(B) ∪ FOLLOW(A):
            symbol_set__union(user->follow_set, follow_set);
        } else {
            // FOLLOW(B) = FOLLOW(B) ∪ FIRST(next grammar symbol),
            // where B = nonterminal
            // compute FIRST(next grammar symbol)
            int next = *production__get(*data, i + 1);
            node_t *first_set = compute_first_set(user->g, next);
            // FOLLOW(B) = FOLLOW(B) ∪ FIRST(next):
            symbol_set__union(user->follow_set, first_set);
        }
    }
}

node_t *compute_follow_set(grammar_t g, int nonterminal)
{
    node_t *follow_set = malloc(sizeof(node_t));
    *follow_set = NULL;

    // add t_eof to follow(S' aka start), only has t_eof as follow
    if (nonterminal == grammar__num_terminals(g)) {
        symbol_queue__insert(follow_set, grammar__num_terminals(g) - 1);
        return follow_set;
    }

    struct info_compute_follow_set icfs0 = { follow_set, nonterminal, g };
    production_path__access(U_QT, grammar__productions(g), &icfs0,
        production_path__predicate_1, compute_follow_set_apply);

    return follow_set;
}
