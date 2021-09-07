#ifndef _LIBSAPLING_TYPED_EQUIVALENCE_CLASS_H_
#define _LIBSAPLING_TYPED_EQUIVALENCE_CLASS_H_

/**
 * @file typed_equivalence_class.h
 * @brief Typed equivalence class interface implementation
 */

#include "libsapling/dm/path.h"
#include "libsapling/dm/typed/typed_adapters.h"

#define IMPLEMENT_TYPED_EQUIVALENCE_CLASS(SYM, TYPE, FPF, EQU)                \
                                                                              \
IMPLEMENT_TYPED_ADAPTERS(SYM, TYPE, equivalence_class)                        \
                                                                              \
static                                                                        \
TYPE SYM##__access_representative(node_t *ref)                                \
{                                                                             \
    return *(TYPE *)equivalence_class__access_representative(ref);            \
}                                                                             \
                                                                              \
static                                                                        \
void SYM##__access_non_representatives(enum qt qt, node_t *ref, void *info,   \
    SYM##__predicate_t predicate, SYM##__apply_t apply)                       \
{                                                                             \
    TYPE *data = info;                                                        \
    struct SYM##__adapt a = { sizeof(TYPE), data, info, predicate, apply };   \
    equivalence_class__access_non_representatives(qt, ref, &a,                \
    SYM##__predicate_adapter, SYM##__apply_adapter);                          \
}                                                                             \
                                                                              \
static                                                                        \
void SYM##__insert(node_t *ref, TYPE val)                                     \
{                                                                             \
    struct info_insert a = { sizeof(TYPE), &val };                            \
    equivalence_class__insert(ref, &a);                                       \
}                                                                             \
                                                                              \
static                                                                        \
void SYM##__print(FILE *stream, node_t *ref)                                  \
{                                                                             \
    path__print(stream, ref, FPF);                                            \
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
                                                                              \
static                                                                        \
int SYM##__in(node_t *ref, TYPE val)                                          \
{                                                                             \
    if (SYM##__access_representative(ref) == val)                             \
        return 1;                                                             \
    int in = 0;                                                               \
    struct SYM##__adapt a = { sizeof(TYPE), &val, &in, SYM##__equ_predicate,  \
        SYM##__a_in };                                                        \
    equivalence_class__access_non_representatives(E_QT, ref, &a,              \
        SYM##__predicate_adapter, SYM##__apply_adapter);                      \
    return in;                                                                \
}

#endif
