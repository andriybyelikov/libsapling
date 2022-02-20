#include <assert.h>
#include "libsapling/cc/parser/follow.h"
#include "test/cc/parser/parser_test_utils.h"

int main(void)
{
    grammar_t g = build_test_grammar();
    node_t *follow_sets = build_test_follow_sets();
    for (int i = grammar__num_terminals(g); i < grammar__num_symbols(g); i++) {
        node_t *follow_set = compute_follow_set(g, i);
        assert(symbol_set__compare(follow_set, &follow_sets[
            i - grammar__num_terminals(g)]));
        /*symbol_path__print(stderr, follow_set);
        fprintf(stderr, " ?= ");
        symbol_path__print(stderr, &follow_sets[
            i - grammar__num_terminals(g)]);
        fprintf(stderr, "\n");*/
    }
}
