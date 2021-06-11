#ifndef LIBSAPLING_QUEUE
#define LIBSAPLING_QUEUE

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
void queue__print_data(FILE *stream, node_t *ref, fpfdata_fn fpfdata);
void queue__dump_dot(FILE *stream, node_t *ref, fpfdata_fn fpfdata);

#endif
