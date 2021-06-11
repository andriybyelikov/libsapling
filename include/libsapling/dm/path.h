#ifndef LIBSAPLING_PATH
#define LIBSAPLING_PATH

/**
 * @file path.h
 * @brief Path implementation
 * @see graph.h
 */

#include "libsapling/dm/graph.h"
#include "libsapling/dm/typed/typed_path.h"

void *path__data(const node_t node);
void path__access(enum qt qt, node_t *ref, void *info, predicate_t predicate,
    apply_t apply);
void path__insert(node_t *ref, void *info, predicate_t predicate);
void path__delete(enum qt qt, node_t *ref, void *info, predicate_t predicate);
void path__print_data(FILE *stream, node_t *ref, fpfdata_fn fpfdata);
void path__dump_dot(FILE *stream, node_t *ref, fpfdata_fn fpfdata);
int path__length(const node_t *ref);

/**
 * @brief Returns the subpath skipping @c pos nodes.
 * @returns the subpath skipping @c pos nodes.
 */
node_t *path__position(node_t *ref, int pos);

#endif
