#include <assert.h>
#include <stdlib.h>
#include "libsapling/cc/parser/grammar.h"
#include "test/parser/parser_test_utils.h"

int main(void)
{
    grammar_t g = build_test_grammar();
    assert(grammar__num_terminals(g) == 6);
    assert(grammar__num_nonterminals(g) == 4);
    assert(grammar__num_symbols(g) == 10);
    for (int i = 0; i < 6; i++) assert(grammar__is_terminal(g, i));
    for (int i = 6; i < 10; i++) assert(!grammar__is_terminal(g, i));
    delete_grammar(g);
}
