#ifndef _LIBSAPLING_STACK_H_
#define _LIBSAPLING_STACK_H_

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

#endif
