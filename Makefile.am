lib_LIBRARIES = libsapling.a
dmdir = $(pkgincludedir)/dm
dm_typeddir = $(pkgincludedir)/dm/typed
ccdir = $(pkgincludedir)/cc
testdir = test

# meta
libsapling_a_SOURCES = src/cc/production.c src/cc/text.c src/cc/lexer/lexer__to_string.c src/cc/lexer/lexer_replace_node.c src/cc/lexer/lexer_attributed_edges.c src/cc/lexer/operators/lexer_kleene_star.c src/cc/lexer/operators/lexer_kleene_plus.c src/cc/lexer/operators/lexer_union.c src/cc/lexer/operators/lexer_concatenation.c src/cc/lexer/lexer_accepting_states.c src/cc/lexer/lexer_minimize.c src/cc/lexer/lexer_insert_state.c src/cc/lexer/lexer.c src/cc/lexer/builders/lexer_copy.c src/cc/lexer/builders/lexer_anything.c src/cc/lexer/builders/lexer_literal.c src/cc/lexer/builders/lexer_class.c src/cc/terminal.c src/cc/parse_tree.c src/cc/parser/first.c src/cc/parser/grammar.c src/cc/parser/follow.c src/cc/parser/aux/symbol_set.c src/dm/equivalence_class.c src/dm/trie.c src/dm/typed/typed_common.c src/dm/queue.c src/dm/stack.c src/dm/avl.c src/dm/path.c src/dm/graph.c

pkginclude_HEADERS = include/libsapling/idiom.h
dm_HEADERS = include/libsapling/dm/stack.h include/libsapling/dm/logic.h include/libsapling/dm/graph.h include/libsapling/dm/queue.h include/libsapling/dm/equivalence_class.h include/libsapling/dm/path.h include/libsapling/dm/trie.h include/libsapling/dm/avl.h
dm_typed_HEADERS = include/libsapling/dm/typed/typed_avl.h include/libsapling/dm/typed/typed_first_out_buffer.h include/libsapling/dm/typed/typed_stack.h include/libsapling/dm/typed/typed_path.h include/libsapling/dm/typed/typed_adapters.h include/libsapling/dm/typed/typed_equivalence_class.h include/libsapling/dm/typed/typed_queue.h include/libsapling/dm/typed/typed_common.h include/libsapling/dm/typed/typed_trie.h
cc_HEADERS = include/libsapling/cc/text.h include/libsapling/cc/terminal.h include/libsapling/cc/production.h include/libsapling/cc/parser/follow.h include/libsapling/cc/parser/first.h include/libsapling/cc/parser/grammar.h include/libsapling/cc/parser/aux/production_set.h include/libsapling/cc/parser/aux/symbol_set.h include/libsapling/cc/lexer.h include/libsapling/cc/parse_tree.h
noinst_HEADERS = src/cc/lexer/lexer_attributed_edges.h src/cc/lexer/lexer_minimize.h src/cc/lexer/lexer_replace_node.h src/cc/lexer/lexer_attributed_edge.h src/cc/lexer/lexer_insert_state.h src/cc/lexer/lexer_ec_def.h src/cc/lexer/lexer_edge_storage.h src/cc/lexer/lexer_accepting_states.h src/dm/first_out_buffer.h

AM_CFLAGS = -std=c11 -pedantic -Wall -Werror -Wno-format -Wno-unused-function -Wno-unused-variable -Wno-unused-but-set-variable -Wno-discarded-qualifiers
AM_CPPFLAGS = -I$(srcdir)/include -I$(srcdir)/src

check_PROGRAMS = test/stack_test test/queue_test test/path_test test/equivalence_class_test test/parse_tree_test test/avl_test test/lexer_test test/trie_test test/production_test test/terminal_test test/print_test test/parser/grammar_test test/parser/first_test test/parser/follow_test
TESTS = $(check_PROGRAMS)

# meta test sources
test_stack_test_SOURCES = test/stack_test.c
test_stack_test_CPPFLAGS = -I$(srcdir)/include
test_stack_test_LDADD = libsapling.a

test_queue_test_SOURCES = test/queue_test.c
test_queue_test_CPPFLAGS = -I$(srcdir)/include
test_queue_test_LDADD = libsapling.a

test_path_test_SOURCES = test/path_test.c
test_path_test_CPPFLAGS = -I$(srcdir)/include
test_path_test_LDADD = libsapling.a

test_equivalence_class_test_SOURCES = test/equivalence_class_test.c
test_equivalence_class_test_CPPFLAGS = -I$(srcdir)/include
test_equivalence_class_test_LDADD = libsapling.a

test_parse_tree_test_SOURCES = test/parse_tree_test.c
test_parse_tree_test_CPPFLAGS = -I$(srcdir)/include
test_parse_tree_test_LDADD = libsapling.a

test_avl_test_SOURCES = test/avl_test.c
test_avl_test_CPPFLAGS = -I$(srcdir)/include
test_avl_test_LDADD = libsapling.a

test_lexer_test_SOURCES = test/lexer_test.c
test_lexer_test_CPPFLAGS = -I$(srcdir)/include
test_lexer_test_LDADD = libsapling.a

test_trie_test_SOURCES = test/trie_test.c
test_trie_test_CPPFLAGS = -I$(srcdir)/include
test_trie_test_LDADD = libsapling.a

test_production_test_SOURCES = test/production_test.c
test_production_test_CPPFLAGS = -I$(srcdir)/include
test_production_test_LDADD = libsapling.a

test_terminal_test_SOURCES = test/terminal_test.c
test_terminal_test_CPPFLAGS = -I$(srcdir)/include
test_terminal_test_LDADD = libsapling.a

test_print_test_SOURCES = test/print_test.c
test_print_test_CPPFLAGS = -I$(srcdir)/include
test_print_test_LDADD = libsapling.a

test_parser_grammar_test_SOURCES = test/parser/grammar_test.c test/parser/parser_test_utils.c
test_parser_grammar_test_CPPFLAGS = -I$(srcdir)/include
test_parser_grammar_test_LDADD = libsapling.a

test_parser_first_test_SOURCES = test/parser/first_test.c test/parser/parser_test_utils.c
test_parser_first_test_CPPFLAGS = -I$(srcdir)/include
test_parser_first_test_LDADD = libsapling.a

test_parser_follow_test_SOURCES = test/parser/follow_test.c test/parser/parser_test_utils.c
test_parser_follow_test_CPPFLAGS = -I$(srcdir)/include
test_parser_follow_test_LDADD = libsapling.a

