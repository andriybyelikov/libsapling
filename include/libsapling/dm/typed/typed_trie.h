#ifndef LIBSAPLING_TYPED_TRIE
#define LIBSAPLING_TYPED_TRIE

/**
 * @file typed_trie.h
 * @brief Typed trie interface implementation
 */

#include <stdlib.h>
#include "libsapling/dm/typed/typed_adapters.h"

#define IMPLEMENT_TYPED_TRIE(SYM, TYPE, FPF)                                  \
                                                                              \
IMPLEMENT_TYPED_ADAPTERS(SYM, TYPE, trie)                                     \
                                                                              \
static                                                                        \
void SYM##__access(enum qt qt, node_t *ref, const char *key, void *info,      \
    SYM##__predicate_fn predicate, SYM##__apply_fn apply)                     \
{                                                                             \
    struct SYM##__adapt a = { 0, NULL, info, predicate, apply };              \
    trie__access(qt, ref, key, &a, SYM##__predicate_adapter,                  \
        SYM##__apply_adapter);                                                \
}                                                                             \
                                                                              \
static                                                                        \
void SYM##__insert(node_t *ref, const char *key, TYPE val)                    \
{                                                                             \
    TYPE *data = malloc(sizeof(TYPE));                                        \
    *data = val;                                                              \
    trie__insert(ref, key, data);                                             \
}                                                                             \
                                                                              \
static                                                                        \
void SYM##__delete(node_t *ref, const char *key)                              \
{                                                                             \
    trie__delete(ref, key);                                                   \
}                                                                             \
                                                                              \
static                                                                        \
void SYM##__print_data(FILE *stream, node_t *ref)                             \
{                                                                             \
    trie__print_data(stream, ref, FPF);                                       \
}                                                                             \
                                                                              \
static                                                                        \
void SYM##__dump_dot(FILE *stream, node_t *ref)                               \
{                                                                             \
    trie__dump_dot(stream, ref, FPF);                                         \
}

#endif
