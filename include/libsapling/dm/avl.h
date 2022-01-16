#ifndef _LIBSAPLING_AVL_H_
#define _LIBSAPLING_AVL_H_

/**
 * @file avl.h
 * @brief AVL tree implementation
 * @see graph.h
 * 
 * @attention
 * - With universal quantification the nodes are visited in inorder.
 * - The comparator only needs to be specified with existential quantification,
 *   otherwise it can just be left as NULL.
 * - Deletion with universal quantification is not defined for this
 *   implementation.
 */

#include "libsapling/dm/graph.h"
#include "libsapling/dm/typed/typed_avl.h"

void *avl__data(const node_t node);
void avl__access(enum qt qt, node_t *ref, void *info, predicate_t predicate,
    apply_t apply, cmp_t comparator);
void avl__insert(node_t *ref, void *info, predicate_t predicate,
    cmp_t comparator);
void avl__delete(node_t *ref, void *info, predicate_t predicate,
    cmp_t comparator);
void avl__print(FILE *stream, node_t *ref, fpfdata_t fpfdata);
void avl__println(FILE *stream, node_t *ref, fpfdata_t fpfdata);
void avl__dump_dot(FILE *stream, node_t *ref, fpfdata_t fpfdata);
int avl__length(const node_t *ref);

#endif
