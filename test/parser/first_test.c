#include <assert.h>
#include "libsapling/cc/parser/first.h"
#include "libsapling/cc/parser/aux/symbol_set.h"
#include "test/parser/parser_test_utils.h"

int main(void)
{
    grammar_t g = build_test_grammar();
    node_t *first_sets = build_test_first_sets();
    for (int i = 0; i < grammar__num_symbols(g); i++) {
        node_t *first_set = compute_first_set(g, i);
        assert(symbol_set__compare(first_set, &first_sets[i]));
        /*symbol_path__print_data(stderr, first_set);
        fprintf(stderr, "\n");
        symbol_path__print_data(stderr, &first_sets[i]);
        fprintf(stderr, "\n");*/
    }
}
