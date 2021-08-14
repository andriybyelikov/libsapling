#ifndef _LIBSAPLING_QUEUE_H_
#define _LIBSAPLING_QUEUE_H_

/**
 * @file queue.h
 * @brief Queue implementation
 * @see graph.h
 */

#include "libsapling/dm/graph.h"
#include "libsapling/dm/typed/typed_queue.h"

void *queue__access(node_t *ref);
void queue__insert(node_t *ref, void *info);
void queue__delete(node_t *ref);

#endif
