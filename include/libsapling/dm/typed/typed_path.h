#ifndef _LIBSAPLING_TYPED_PATH_H_
#define _LIBSAPLING_TYPED_PATH_H_

/**
 * @file typed_path.h
 * @brief Typed path interface implementation
 */

#include "libsapling/dm/typed/typed_adapters.h"

#define IMPLEMENT_TYPED_PATH(SYM, TYPE, FPF, EQU)                             \
                                                                              \
IMPLEMENT_TYPED_ADAPTERS(SYM, TYPE, path)                                     \
                                                                              \
static                                                                        \
void SYM##__access(enum qt qt, node_t *ref, void *info,                       \
    SYM##__predicate_t predicate, SYM##__apply_t apply)                       \
{                                                                             \
    struct SYM##__adapt a = { 0, NULL, info, predicate, apply };              \
    path__access(qt, ref, &a, SYM##__predicate_adapter, SYM##__apply_adapter);\
}                                                                             \
                                                                              \
static                                                                        \
void SYM##__insert(node_t *ref, TYPE val, SYM##__predicate_t predicate)       \
{                                                                             \
    struct SYM##__adapt a = { sizeof(TYPE), &val, NULL, predicate };          \
    path__insert(ref, &a, SYM##__predicate_adapter);                          \
}                                                                             \
                                                                              \
static                                                                        \
void SYM##__delete(enum qt qt, node_t *ref, void *info,                       \
    SYM##__predicate_t predicate)                                             \
{                                                                             \
    struct SYM##__adapt a = { 0, NULL, info, predicate, NULL };               \
    path__delete(qt, ref, &a, SYM##__predicate_adapter);                      \
}                                                                             \
                                                                              \
static                                                                        \
void SYM##__print(FILE *stream, node_t *ref)                                  \
{                                                                             \
    path__print(stream, ref, FPF);                                            \
}                                                                             \
                                                                              \
static                                                                        \
void SYM##__println(FILE *stream, node_t *ref)                                \
{                                                                             \
    path__println(stream, ref, FPF);                                          \
}                                                                             \
                                                                              \
static                                                                        \
void SYM##__dump_dot(FILE *stream, node_t *ref)                               \
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
int SYM##__equ_predicate(const TYPE *data, void *info)                        \
{                                                                             \
    struct info_insert *ins = info;                                           \
                                                                              \
    return EQU(ins->data, data);                                              \
}                                                                             \
static                                                                        \
void SYM##__a_in(UNUSED TYPE *data, void *info)                               \
{                                                                             \
    CAST_USER_INFO(int *, in, info);                                          \
                                                                              \
    *in = 1;                                                                  \
}                                                                             \
static                                                                        \
int SYM##__in(node_t *ref, TYPE val)                                          \
{                                                                             \
    int in = 0;                                                               \
    struct SYM##__adapt a = { sizeof(TYPE), &val, &in, SYM##__equ_predicate,  \
        SYM##__a_in };                                                        \
    path__access(E_QT, ref, &a, SYM##__predicate_adapter,                     \
        SYM##__apply_adapter);                                                \
    return in;                                                                \
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
