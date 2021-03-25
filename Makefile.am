lib_LIBRARIES = libsapling.a
dmdir = $(pkgincludedir)/dm
ccdir = $(pkgincludedir)/cc
testdir = test

# meta
libsapling_a_SOURCES = src/cc/production.c src/cc/text.c src/cc/terminal.c src/cc/parse_tree.c src/dm/equivalence_class.c src/dm/queue.c src/dm/stack.c src/dm/avl.c src/dm/path.c src/dm/graph.c

pkginclude_HEADERS = include/libsapling/idiom.h
dm_HEADERS = include/libsapling/dm/stack.h include/libsapling/dm/logic.h include/libsapling/dm/graph.h include/libsapling/dm/typed/typed_avl.h include/libsapling/dm/typed/typed_stack.h include/libsapling/dm/typed/typed_path.h include/libsapling/dm/typed/typed_adapters.h include/libsapling/dm/typed/typed_equivalence_class.h include/libsapling/dm/typed/typed_queue.h include/libsapling/dm/typed/typed_first_out_buffer.h include/libsapling/dm/queue.h include/libsapling/dm/equivalence_class.h include/libsapling/dm/path.h include/libsapling/dm/avl.h
cc_HEADERS = include/libsapling/cc/text.h include/libsapling/cc/terminal.h include/libsapling/cc/production.h include/libsapling/cc/parse_tree.h
noinst_HEADERS = src/dm/first_out_buffer.h

AM_CFLAGS = -std=c11 -pedantic
AM_CPPFLAGS = -I$(srcdir)/include -I$(srcdir)/src

test_HEADERS = test/test_utils.h
check_PROGRAMS = test/stack_test test/queue_test test/path_test test/equivalence_class_test test/parse_tree_test test/avl_test
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

