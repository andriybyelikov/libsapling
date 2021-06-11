#ifndef LIBSAPLING_TYPED_AVL
#define LIBSAPLING_TYPED_AVL

/**
 * @file typed_avl.h
 * @brief Typed AVL interface implementation
 */

#include "libsapling/dm/typed/typed_adapters.h"

#define IMPLEMENT_TYPED_AVL(SYM, TYPE, CMP, EQU, FPF)                                 \
                                                                              \
IMPLEMENT_TYPED_ADAPTERS(SYM, TYPE, avl)                                              \
                                                                              \
static                                                                        \
void SYM##__access(enum qt qt, node_t *ref, void *info,                          \
    SYM##__predicate_t predicate, SYM##__apply_t apply)                     \
{                                                                             \
    TYPE *dat = info;   \
    struct SYM##__adapt a = { sizeof(TYPE), dat, info, predicate, apply };                              \
    avl__access(qt, ref, &a, SYM##__predicate_adapter, SYM##__apply_adapter, CMP); \
}                                                                             \
                                                                              \
                                                                              \
                                                                              \
static                                                                        \
int SYM##__cmp_predicate(const TYPE *data, void *info)                        \
{                                                                             \
    struct info_insert *ins = info;                                           \
                                                                              \
    return CMP(ins->data, data) < 0 ? 1 : 0;                                  \
}                                                                             \
                                                                              \
static                                                                        \
int SYM##__equ_predicate(const TYPE *data, void *info)                        \
{                                                                             \
    struct info_insert *ins = info;                                           \
                                                                              \
    return EQU(ins->data, data);                                  \
}    \
static                                                                        \
void SYM##__a_in(UNUSED TYPE *data, void *info)                               \
{                                                                             \
    struct info_insert *i = info;                                             \
    int *in = i->info;                                         \
                                                                              \
    *in = 1;                                                               \
}                                                                             \
                                                                              \
static                                                                        \
int SYM##__in(node_t *ref, TYPE val)                                          \
{                                                                             \
    int in = 0;                                                               \
    struct SYM##__adapt a = { sizeof(TYPE), &val, &in, SYM##__equ_predicate,  \
        SYM##__a_in };                                                        \
    avl__access(E_QT, ref, &a, SYM##__predicate_adapter,                      \
        SYM##__apply_adapter, CMP);                                           \
    return in;                                                                \
}                                                                             \
                                                                              \
static                                                                        \
void SYM##__insert(node_t *ref, TYPE val, SYM##__predicate_t predicate)      \
{                                                                             \
    struct info_insert info = { sizeof(TYPE), &val, NULL, predicate, NULL };  \
    avl__insert(ref, &info, SYM##__predicate_adapter, CMP);                   \
}                                                                             \
                                                                              \
static                                                                        \
void SYM##__delete(node_t *ref, TYPE val, SYM##__predicate_t predicate)      \
{                                                                             \
    struct SYM##__adapt a = { sizeof(TYPE), &val, NULL, predicate, NULL };    \
    avl__delete(ref, &a, SYM##__predicate_adapter, CMP);                      \
}                                                                             \
                                                                              \
static                                                                        \
void SYM##__print_data(FILE *stream, node_t *ref)                             \
{                                                                             \
    avl__print_data(stream, ref, FPF);                                        \
}                                                                             \
                                                                              \
static                                                                        \
void SYM##__dump_dot(FILE *stream, node_t *ref)                               \
{                                                                             \
    avl__dump_dot(stream, ref, FPF);                                          \
}

#endif
