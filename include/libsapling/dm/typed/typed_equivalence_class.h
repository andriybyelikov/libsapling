#ifndef LIBSAPLING_TYPED_EQUIVALENCE_CLASS
#define LIBSAPLING_TYPED_EQUIVALENCE_CLASS

/**
 * @file typed_equivalence_class.h
 * @brief Typed equivalence class interface implementation
 */

#include "libsapling/dm/typed/typed_adapters.h"

#define IMPLEMENT_TYPED_EQUIVALENCE_CLASS(SYM, TYPE, FPF, EQU)                     \
                                                                              \
static                                                                        \
void SYM##__insert(node_t *ref, TYPE val)                                   \
{                                                                             \
    struct info_insert info = { sizeof(TYPE), &val, NULL, NULL };        \
    equivalence_class__insert(ref, &info);                                    \
}                                                                             \
                                                                              \
static                                                                        \
TYPE SYM##__access_representative(node_t *ref)                                \
{                                                                             \
    return *(TYPE *)equivalence_class__access_representative(ref);            \
}                                                                             \
                                                                              \
                                                                              \
IMPLEMENT_TYPED_ADAPTERS(SYM, TYPE, equivalence_class)                        \
                                                                              \
static                                                                        \
void SYM##__access_non_representatives(enum qt qt, node_t *ref, void *info,   \
    SYM##__predicate_fn predicate, SYM##__apply_fn apply)                     \
{                                                                             \
    TYPE *dat = info;   \
    struct SYM##__adapt a = { sizeof(TYPE), dat, info, predicate, apply };                              \
    equivalence_class__access_non_representatives(qt, ref, &a, SYM##__predicate_adapter, SYM##__apply_adapter);\
}                                                                             \
                                                                              \
                                                                              \
static                                                                        \
void SYM##__print_data(FILE *stream, node_t *ref)                       \
{                                                                             \
    equivalence_class__print_data(stream, ref, FPF);                          \
}                                                                             \
                                                                              \
static                                                                        \
void SYM##__dump_dot(FILE *stream, node_t *ref)                         \
{                                                                             \
    equivalence_class__dump_dot(stream, ref, FPF);                            \
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
static                                                                        \
int SYM##__in(node_t *ref, TYPE val)                                         \
{                                                                             \
    if (SYM##__access_representative(ref) == val) \
        return 1; \
    int in = 0; \
    struct SYM##__adapt a = { sizeof(TYPE), &val, &in, SYM##__equ_predicate, SYM##__a_in }; \
    equivalence_class__access_non_representatives(E_QT, ref, &a, SYM##__predicate_adapter,                      \
        SYM##__apply_adapter);                                           \
    return in;                                                                \
}

#endif
