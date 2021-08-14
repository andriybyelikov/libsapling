#ifndef _LIBSAPLING_EQUIVALENCE_CLASS_H_
#define _LIBSAPLING_EQUIVALENCE_CLASS_H_

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
    void *info, predicate_t predicate, apply_t apply);

#endif
