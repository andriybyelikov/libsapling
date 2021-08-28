#ifndef _LIBSAPLING_PARSER_TEST_UTILS_H_
#define _LIBSAPLING_PARSER_TEST_UTILS_H_

#include "libsapling/cc/parser/action_table.h"
#include "libsapling/cc/parser/goto_table.h"

grammar_t build_test_grammar(void);
node_t *build_test_first_sets(void);
node_t *build_test_follow_sets(void);
node_t *build_test_set_of_sets_of_lr0_items(grammar_t g);
action_table_t build_test_action_table(grammar_t g, node_t *C);
goto_table_t build_test_goto_table(grammar_t g, node_t *C);
int symbol_set__compare(node_t *a, node_t *b);
int compare_slr_tables(grammar_t g, node_t *C, action_table_t at0,
    goto_table_t gt0, action_table_t at1, goto_table_t gt1);

#endif
