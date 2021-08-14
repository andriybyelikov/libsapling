#ifndef _LIBSAPLING_TYPED_ADAPTERS_H_
#define _LIBSAPLING_TYPED_ADAPTERS_H_

/**
 * @file typed_adapters.h
 * @brief Implementation of typed adapters for predicate and apply functions
 * 
 * Adapters allow predicates and apply functions to be defined without having
 * to expose the node and the implementation information. Adapters provide
 * these predicate and apply functions with a pointer to node's data cast to
 * the appropiate type and the pointer to the user information directly,
 * instead of providing a pointer to the node and the information stack proper.
 * This reduces the number of casts the user has to write when writing a
 * predicate or an apply function.
 */

#include "libsapling/dm/graph.h"

/**
 * @brief Creates a variable of the specified type with the user information
 * proper.
 * 
 * @param TYPE Type of the variable
 * @param VAR Name of the variable
 * @param INFO Name of function parameter that points to the user information
 */
#define CAST_USER_INFO(TYPE, VAR, INFO) \
TYPE VAR = ((struct info_insert *)INFO)->info

/**
 * @brief Pastes all definitions that implement typed adapters
 * 
 * @attention This macro is only intended to be used when defining a typed
 * interface for a set implementation.
 * 
 * @param SYM Prefix symbol for the adapter definitions
 * @param TYPE Type with which the adapters will be dealing with
 * @param IMPL Which set implementation's data will be accessed
 */
#define IMPLEMENT_TYPED_ADAPTERS(SYM, TYPE, IMPL)                             \
                                                                              \
typedef int (*SYM##__predicate_t)(const TYPE *data, void *info);              \
typedef void (*SYM##__apply_t)(TYPE *data, void *info);                       \
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
    SYM##__predicate_t predicate;                                             \
    SYM##__apply_t apply;                                                     \
};                                                                            \
                                                                              \
static                                                                        \
int SYM##__predicate_adapter(const node_t *ref,                               \
    const struct info_stack *info)                                            \
{                                                                             \
    node_t node = *ref;                                                       \
    struct SYM##__adapt *user = info->user;                                   \
                                                                              \
    return user->predicate((const TYPE *)IMPL##__data(node), user);           \
}                                                                             \
                                                                              \
static                                                                        \
void SYM##__apply_adapter(node_t *ref, const struct info_stack *info)         \
{                                                                             \
    node_t node = *ref;                                                       \
    struct SYM##__adapt *user = info->user;                                   \
                                                                              \
    if (*ref != NULL)                                                         \
        user->apply((TYPE *)IMPL##__data(node), user);                        \
}

#endif
