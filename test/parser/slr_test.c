#include <assert.h>
#include "libsapling/cc/parser/slr.h"
#include "libsapling/cc/parser/aux/set_of_sets_of_lr0_items.h"
#include "test/parser/parser_test_utils.h"

int main(void)
{
    grammar_t g = build_test_grammar();
    node_t *expected_set_of_sets_of_lr0_items =
        build_test_set_of_sets_of_lr0_items(g);
    node_t *computed_set_of_sets_of_lr0_items =
        slr__build_set_of_sets_of_lr0_items(g);
    assert(set_of_sets_of_lr0_items__equals(
        &expected_set_of_sets_of_lr0_items,
        &computed_set_of_sets_of_lr0_items));
    /*set_of_sets_of_lr0_items_path__print_data(stderr,
        expected_set_of_sets_of_lr0_items);
    fprintf(stderr, "\n");
    set_of_sets_of_lr0_items_path__print_data(stderr,
        computed_set_of_sets_of_lr0_items);
    fprintf(stderr, "\n");*/
}
