#include <assert.h>
#include "libsapling/cc/parser/slr.h"
#include "libsapling/cc/parser/defs/set_of_sets_of_lr0_items.h"
#include "test/cc/parser/parser_test_utils.h"

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
    /*set_of_sets_of_lr0_items_path__print(stderr,
        expected_set_of_sets_of_lr0_items);
    fprintf(stderr, "\n");
    set_of_sets_of_lr0_items_path__print(stderr,
        computed_set_of_sets_of_lr0_items);
    fprintf(stderr, "\n");*/

    // Here the computed action and goto tables must be build using the same
    // set of sets of LR(0) items in order for the states to have the same
    // enumeration.
    action_table_t expected_action_table = build_test_action_table(g,
        expected_set_of_sets_of_lr0_items);
    goto_table_t expected_goto_table = build_test_goto_table(g,
        expected_set_of_sets_of_lr0_items);
    action_table_t computed_action_table = action_table__build(g,
        expected_set_of_sets_of_lr0_items);
    goto_table_t computed_goto_table = goto_table__build(g,
        expected_set_of_sets_of_lr0_items);
    assert(compare_slr_tables(g, expected_set_of_sets_of_lr0_items,
        expected_action_table, expected_goto_table,
        computed_action_table, computed_goto_table));
    /*slr__print_tables(stderr, action_table, goto_table,
        expected_set_of_sets_of_lr0_items, g);*/
}
