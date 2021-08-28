#ifndef _LIBSAPLING_PARSER_SLR_H_
#define _LIBSAPLING_PARSER_SLR_H_

/**
 * @file slr.h
 * @brief Simple LR implementation
 */

#include "libsapling/dm/graph.h"
#include "libsapling/cc/parser/grammar.h"

/**
 * @brief Constructs the set of sets of LR(0) items from a grammar.
 * 
 * @returns a path (set of sets of LR(0) items) with the constructed sets of
 * LR(0) items.
 */
node_t *slr__build_set_of_sets_of_lr0_items(grammar_t g);

#endif
