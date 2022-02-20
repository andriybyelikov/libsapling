#include <stdlib.h>
#include "libsapling/dm/graph.h"
#include "libsapling/cc/parser/grammar.h"
#include "libsapling/cc/parser/defs/production_set.h"

struct grammar {
    node_t *productions;
    int num_terminals;
    int num_symbols;
};

grammar_t new_grammar(node_t *productions, int num_terminals, int num_symbols)
{
    grammar_t g = malloc(sizeof(struct grammar));
    g->productions = productions;
    g->num_terminals = num_terminals;
    g->num_symbols = num_symbols;
    return g;
}

void delete_grammar(grammar_t g)
{
    free(g);
}

node_t *grammar__productions(grammar_t g)
{
    return g->productions;
}

int grammar__num_terminals(grammar_t g)
{
    return g->num_terminals;
}

int grammar__num_nonterminals(grammar_t g)
{
    return g->num_symbols - g->num_terminals;
}

int grammar__num_symbols(grammar_t g)
{
    return g->num_symbols;
}

int grammar__is_terminal(grammar_t g, int symbol)
{
    return symbol < g->num_terminals;
}


struct info_compute_set {
    node_t *set;
    int id;
    grammar_t g;
};


node_t *compute_first_set(grammar_t g, int symbol);

static
void compute_first_set_apply(production_t *data, void *info)
{
    CAST_USER_INFO(struct info_compute_set *, user, info);

    symbol_set__union(user->set,
        compute_first_set(user->g, *production__get(*data, 0)));
}

static
int compute_first_set_predicate(const production_t *data, void *info)
{
    CAST_USER_INFO(struct info_compute_set *, user, info);

    return production__id(*data) == user->id
        && *production__get(*data, 0) != user->id;
}

node_t *compute_first_set(grammar_t g, int symbol)
{
    node_t *first_set = malloc(sizeof(node_t));
    *first_set = NULL;

    if (grammar__is_terminal(g, symbol)) { 
        symbol_queue__insert(first_set, symbol);
    } else {
        struct info_compute_set icfs0 = { first_set, symbol, g };
        production_path__access(U_QT, grammar__productions(g), &icfs0,
            compute_first_set_predicate, compute_first_set_apply);
    }

    return first_set;
}


node_t *compute_follow_set(grammar_t g, int nonterminal);

static
void compute_follow_set_apply(production_t *data, void *info)
{
    CAST_USER_INFO(struct info_compute_set *, user, info);

    for (int i = 0; i < production__len(*data); i++) {
        int sym = *production__get(*data, i);
        if (sym != user->id)
            continue;
        if (i == production__len(*data) - 1) {
            if (user->id != production__id(*data)) {
                // FOLLOW(B) = FOLLOW(B) ∪ FOLLOW(A), where B = nonterminal
                // compute FOLLOW(A):
                node_t *follow_set = compute_follow_set(user->g,
                    production__id(*data));
                // FOLLOW(B) = FOLLOW(B) ∪ FOLLOW(A):
                symbol_set__union(user->set, follow_set);
            }
        } else {
            // FOLLOW(B) = FOLLOW(B) ∪ FIRST(next grammar symbol),
            // where B = nonterminal
            // compute FIRST(next grammar symbol)
            int next = *production__get(*data, i + 1);
            node_t *first_set = compute_first_set(user->g, next);
            // FOLLOW(B) = FOLLOW(B) ∪ FIRST(next):
            symbol_set__union(user->set, first_set);
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

    struct info_compute_set icfs0 = { follow_set, nonterminal, g };
    production_path__access(U_QT, grammar__productions(g), &icfs0,
        production_path__predicate_1, compute_follow_set_apply);

    return follow_set;
}
