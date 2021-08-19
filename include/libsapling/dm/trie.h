#ifndef _LIBSAPLING_TRIE_H_
#define _LIBSAPLING_TRIE_H_

/**
 * @file trie.h
 * @brief Trie implementation
 * @see graph.h
 * 
 * @attention
 * - The key only needs to be specified with existential quantification,
 *   otherwise it can just be left as NULL.
 * - Accessing with `E_QT` and `predicate_1`, and managing to execute the apply
 *   function means that the key exists, therefore there is no need for an
 *   `equ_predicate` for the key. A predicate different from `predicate_1` is
 *   only useful when you want to check non-key values.
 */

#include "libsapling/dm/graph.h"
#include "libsapling/dm/typed/typed_trie.h"

void *trie__data(const node_t node);
void trie__access(enum qt qt, node_t *ref, const char *key, void *info,
    predicate_t predicate, apply_t apply);
void trie__insert(node_t *ref, const char *key, void *info);
void trie__delete(node_t *ref, const char *key);
void trie__print_data(FILE *stream, node_t *ref, fpfdata_t fpfdata);
int trie__length(const node_t *ref);

#endif
