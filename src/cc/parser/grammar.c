#include <stdlib.h>
#include "libsapling/dm/graph.h"
#include "libsapling/cc/parser/grammar.h"

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
