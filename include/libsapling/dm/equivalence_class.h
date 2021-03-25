#ifndef LIBSAPLING_EQUIVALENCE_CLASS
#define LIBSAPLING_EQUIVALENCE_CLASS

/**
 * @file equivalence_class.h
 * @brief Equivalence class implementation
 * @see graph.h
 */

#include "libsapling/dm/graph.h"
#include "libsapling/dm/typed/typed_equivalence_class.h"

void *equivalence_class__data(node_t ref);
void equivalence_class__insert(node_t *ref, void *info);
void *equivalence_class__access_representative(node_t *ref);
void equivalence_class__access_non_representatives(enum qt qt, node_t *ref,
    void *info, predicate_fn predicate, apply_fn apply);
void equivalence_class__print_data(FILE *stream, node_t *ref,
    fpfdata_fn fpfdata);
void equivalence_class__dump_dot(FILE *stream, node_t *ref,
    fpfdata_fn fpfdata);

#endif
