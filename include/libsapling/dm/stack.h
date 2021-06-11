#ifndef LIBSAPLING_STACK
#define LIBSAPLING_STACK

/**
 * @file stack.h
 * @brief Stack implementation
 * @see graph.h
 */

#include "libsapling/dm/graph.h"
#include "libsapling/dm/typed/typed_stack.h"

void *stack__access(node_t *ref);
void stack__insert(node_t *ref, void *info);
void stack__delete(node_t *ref);
void stack__print_data(FILE *stream, node_t *ref, fpfdata_fn fpfdata);
void stack__dump_dot(FILE *stream, node_t *ref, fpfdata_fn fpfdata);

#endif
