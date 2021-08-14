#ifndef _LIBSAPLING_TYPED_FIRST_OUT_BUFFER_H_
#define _LIBSAPLING_TYPED_FIRST_OUT_BUFFER_H_

/**
 * @file typed_first_out_buffer.h
 * @brief Metaimplementation of the typed stack and queue interfaces
 */

#include "libsapling/dm/path.h"

/**
 * @brief Defines a typed stack implementation
 * 
 * @param SYM Prefix symbol of all implemented functions
 * @param TYPE Type with which the implementation will be dealing with
 * @param FPF fpfdata function that prints the formatted data of the
 * implementation's type
 * @param IMPL Access, insert, delete functions' prefix (stack or queue)
 */
#define IMPLEMENT_TYPED_FIRST_OUT_BUFFER(SYM, TYPE, FPF, IMPL)                \
                                                                              \
static                                                                        \
void SYM##__insert(node_t *ref, TYPE value)                                   \
{                                                                             \
    struct info_insert info = { sizeof(TYPE), &value };                       \
    IMPL##__insert(ref, &info);                                               \
}                                                                             \
                                                                              \
static                                                                        \
TYPE SYM##__access(node_t *ref)                                               \
{                                                                             \
    return *(TYPE *)IMPL##__access(ref);                                      \
}                                                                             \
                                                                              \
static                                                                        \
TYPE SYM##__delete(node_t *ref)                                               \
{                                                                             \
    TYPE retval = *(TYPE *)IMPL##__access(ref);                               \
    IMPL##__delete(ref);                                                      \
    return retval;                                                            \
}                                                                             \
                                                                              \
static                                                                        \
void SYM##__print_data(FILE *stream, node_t *ref)                             \
{                                                                             \
    path__print_data(stream, ref, FPF);                                       \
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
}

#endif
