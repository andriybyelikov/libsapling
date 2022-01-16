#ifndef _LIBSAPLING_TYPED_TRIE_H_
#define _LIBSAPLING_TYPED_TRIE_H_

/**
 * @file typed_trie.h
 * @brief Typed trie interface implementation
 */

#include <stdlib.h>
#include "libsapling/dm/typed/typed_adapters.h"
#include "libsapling/cc/lexer.h"

#define IMPLEMENT_TYPED_TRIE(SYM, TYPE, FPF)                                  \
                                                                              \
IMPLEMENT_TYPED_ADAPTERS(SYM, TYPE, trie)                                     \
                                                                              \
static                                                                        \
void SYM##__access(enum qt qt, node_t *ref, const char *key, void *info,      \
    SYM##__predicate_t predicate, SYM##__apply_t apply)                       \
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
void SYM##__print(FILE *stream, node_t *ref)                                  \
{                                                                             \
    trie__print(stream, ref, FPF);                                            \
}                                                                             \
                                                                              \
static                                                                        \
void SYM##__println(FILE *stream, node_t *ref)                                \
{                                                                             \
    trie__println(stream, ref, FPF);                                          \
}                                                                             \
                                                                              \
static                                                                        \
void SYM##__dump_dot(FILE *stream, node_t *ref)                               \
{                                                                             \
    lexer__dump_dot(stream, ref, FPF);                                        \
}                                                                             \
                                                                              \
static                                                                        \
int SYM##__length(const node_t *ref)                                          \
{                                                                             \
    return trie__length(ref);                                                 \
}                                                                             \
                                                                              \
                                                                              \
static                                                                        \
void SYM##__found_key(node_t *ref, const struct info_stack *info)             \
{                                                                             \
    int *found = info->user;                                                  \
                                                                              \
    *found = 1;                                                               \
}                                                                             \
                                                                              \
static                                                                        \
int SYM##__in(node_t *ref, const char *key)                                   \
{                                                                             \
    int in = 0;                                                               \
    trie__access(E_QT, ref, key, &in, predicate_1, SYM##__found_key);         \
    return in;                                                                \
}

#endif
