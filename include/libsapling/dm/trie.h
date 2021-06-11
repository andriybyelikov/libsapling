#ifndef LIBSAPLING_TRIE
#define LIBSAPLING_TRIE

/**
 * @file trie.h
 * @brief Trie implementation
 * @see graph.h
 */

#include "libsapling/dm/graph.h"
#include "libsapling/dm/typed/typed_trie.h"

void *trie__data(const node_t node);
void trie__access(enum qt qt, node_t *ref, const char *key, void *info,
    predicate_t predicate, apply_t apply);
void trie__insert(node_t *ref, const char *key, void *info);
void trie__delete(node_t *ref, const char *key);
void trie__print_data(FILE *stream, node_t *ref, fpfdata_fn fpfdata);
void trie__dump_dot(FILE *stream, node_t *ref, fpfdata_fn fpfdata);

#endif
