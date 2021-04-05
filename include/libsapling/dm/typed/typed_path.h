#ifndef LIBSAPLING_TYPED_PATH
#define LIBSAPLING_TYPED_PATH

/**
 * @file typed_path.h
 * @brief Typed path interface implementation
 */

#include "libsapling/dm/typed/typed_adapters.h"

#define IMPLEMENT_TYPED_PATH(SYM, TYPE, FPF)                                  \
                                                                              \
IMPLEMENT_TYPED_ADAPTERS(SYM, TYPE, path)                                     \
                                                                              \
static                                                                        \
void SYM##__access(enum qt qt, node_t *ref, void *info,                       \
    SYM##__predicate_fn predicate, SYM##__apply_fn apply)                     \
{                                                                             \
    struct SYM##__adapt a = { 0, NULL, info, predicate, apply };              \
    path__access(qt, ref, &a, SYM##__predicate_adapter, SYM##__apply_adapter);\
}                                                                             \
                                                                              \
static                                                                        \
void SYM##__insert(node_t *ref, TYPE val, SYM##__predicate_fn predicate)      \
{                                                                             \
    struct info_insert info = { sizeof(TYPE), &val, NULL, predicate };        \
    path__insert(ref, &info, SYM##__predicate_adapter);                       \
}                                                                             \
                                                                              \
static                                                                        \
void SYM##__delete(enum qt qt, node_t *ref, void *info,                       \
    SYM##__predicate_fn predicate)                                            \
{                                                                             \
    struct info_insert a = { 0, NULL, info, predicate, NULL };               \
    path__delete(qt, ref, &a, SYM##__predicate_adapter);                      \
}                                                                             \
                                                                              \
static                                                                        \
void SYM##__print_data(FILE *stream, node_t *ref)                       \
{                                                                             \
    path__print_data(stream, ref, FPF);                                       \
}                                                                             \
                                                                              \
static                                                                        \
void SYM##__dump_dot(FILE *stream, node_t *ref)                         \
{                                                                             \
    path__dump_dot(stream, ref, FPF);                                         \
}                                                                             \
                                                                              \
static                                                                        \
int SYM##__length(const node_t *ref)                                          \
{                                                                             \
    return path__length(ref);                                                 \
}                                                                             \
                                                                              \
static                                                                        \
TYPE *SYM##__position(node_t *ref, int pos)                                   \
{                                                                             \
    node_t *subpath = path__position(ref, pos);                               \
    if (subpath != NULL && *subpath != NULL)                                  \
        return (TYPE *)path__data(*subpath);                                  \
    return NULL;                                                              \
}

#endif
