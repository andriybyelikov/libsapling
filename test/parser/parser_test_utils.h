#ifndef _LIBSAPLING_PARSER_TEST_UTILS_H_
#define _LIBSAPLING_PARSER_TEST_UTILS_H_

#include "libsapling/cc/parser/grammar.h"

grammar_t build_test_grammar(void);
node_t *build_test_first_sets(void);
int symbol_set__compare(node_t *a, node_t *b);

#endif
