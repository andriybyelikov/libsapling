#include <assert.h>
#include <stdlib.h>
#include "libsapling/cc/parser/grammar.h"
#include "test/parser/parser_test_utils.h"

/*
 * Example 4.40 from the Dragon Book
 * 
 * E' -> E;
 * E  -> E + T | T;
 * T  -> T * F | F;
 * F  -> ( E ) | id;
 * 
 * 
 * terminals:
 * 0 id
 * 1 +
 * 2 *
 * 3 (
 * 4 )
 * 5 $
 * 
 * nonterminals:
 * 6 E'
 * 7 E
 * 8 T
 * 9 F
 * 
 * productions:
 * 0 [6 -> 7]
 * 1 [7 -> 7 1 8]
 * 2 [7 -> 8]
 * 3 [8 -> 8 2 9]
 * 4 [8 -> 9]
 * 5 [9 -> 3 7 4]
 * 6 [9 -> 0]
 */

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
