#ifndef LIBSAPLING_FIRST_OUT_BUFFER
#define LIBSAPLING_FIRST_OUT_BUFFER

/**
 * @file first_out_buffer.h
 * @brief Metaimplementation of stack and queue
 * 
 * The accesed element will always be the first node, so it is a matter of
 * deciding where to insert new elements.
 */

#include "libsapling/dm/path.h"

/**
 * @brief Inserting new elements at the end means that the elements that were
 *   inserted first will be accessed first.
 */
#define FIRST_IN predicate_0

/**
 * @brief Inserting new elements at the beginning means that the elements that
 *   were inserted last will be accessed first.
 */
#define LAST_IN predicate_1

#define IMPLEMENT_FIRST_OUT_BUFFER(FIRST_OUT, SYM)                            \
                                                                              \
void SYM##__insert(node_t *ref, void *info)                                   \
{                                                                             \
    path__insert(ref, info, FIRST_OUT);                                       \
}                                                                             \
                                                                              \
void SYM##__delete(node_t *ref)                                               \
{                                                                             \
    path__delete(E_QT, ref, NULL, predicate_1);                               \
}                                                                             \
                                                                              \
                                                                              \
struct get_data {                                                             \
    void *data;                                                               \
};                                                                            \
                                                                              \
static                                                                        \
void get_data(node_t *ref, const struct info_stack *info)                     \
{                                                                             \
    if (*ref != NULL) {                                                       \
        struct get_data *user = info->user;                                   \
        user->data = path__data(*ref);                                        \
    }                                                                         \
}                                                                             \
                                                                              \
void *SYM##__access(node_t *ref)                                              \
{                                                                             \
    struct get_data info = { NULL };                                          \
    path__access(E_QT, ref, &info, predicate_1, get_data);                    \
    return info.data;                                                         \
}                                                                             \
                                                                              \
                                                                              \
void SYM##__print_data(FILE *stream, node_t *ref, fpfdata_fn fpfdata)         \
{                                                                             \
    path__print_data(stream, ref, fpfdata);                                   \
}                                                                             \
                                                                              \
void SYM##__dump_dot(FILE *stream, node_t *ref, fpfdata_fn fpfdata)           \
{                                                                             \
    path__dump_dot(stream, ref, fpfdata);                                     \
}

#endif
