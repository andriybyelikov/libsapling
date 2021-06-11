#ifndef LIBSAPLING_TYPED_ADAPTERS
#define LIBSAPLING_TYPED_ADAPTERS

/**
 * @file typed_adapters.h
 * @brief Implementation of typed adapters for predicate and apply functions
 */

#include "libsapling/dm/graph.h"

#define IMPLEMENT_TYPED_ADAPTERS(SYM, TYPE, IMPL)                             \
                                                                              \
typedef int (*SYM##__predicate_t)(const TYPE *data, void *info);             \
typedef void (*SYM##__apply_t)(TYPE *data, void *info);                \
                                                                              \
static                                                                        \
int SYM##__predicate_0(UNUSED const TYPE *data, UNUSED void *info)            \
{                                                                             \
    return 0;                                                                 \
}                                                                             \
                                                                              \
static                                                                        \
int SYM##__predicate_1(UNUSED const TYPE *data, UNUSED void *info)            \
{                                                                             \
    return 1;                                                                 \
}                                                                             \
                                                                              \
struct SYM##__adapt {                                                         \
    size_t size;                                                              \
    void *data;                                                               \
    void *info;                                                               \
    SYM##__predicate_t predicate;                                            \
    SYM##__apply_t apply;                                                    \
};                                                                            \
                                                                              \
static                                                                        \
int SYM##__predicate_adapter(const node_t *ref,                               \
    const struct info_stack *info)                                            \
{                                                                             \
    node_t node = *ref;                                                       \
    struct SYM##__adapt *user = info->user;                                   \
                                                                              \
    return user->predicate((const TYPE *)IMPL##__data(node), user);     \
}                                                                             \
                                                                              \
static                                                                        \
void SYM##__apply_adapter(node_t *ref, const struct info_stack *info)         \
{                                                                             \
    node_t node = *ref;                                                       \
    struct SYM##__adapt *user = info->user;                                   \
    if (*ref != NULL)                                                            \
        user->apply((TYPE *)IMPL##__data(node), user);                \
}

#endif
