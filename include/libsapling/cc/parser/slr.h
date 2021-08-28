#ifndef _LIBSAPLING_PARSER_SLR_H_
#define _LIBSAPLING_PARSER_SLR_H_

/**
 * @file slr.h
 * @brief Simple LR implementation
 */

#include "libsapling/cc/parser/action_table.h"
#include "libsapling/cc/parser/goto_table.h"


/**
 * @brief Constructs the set of sets of LR(0) items from a grammar.
 * 
 * @returns a path (set of sets of LR(0) items) with the constructed sets of
 * LR(0) items.
 */
node_t *slr__build_set_of_sets_of_lr0_items(grammar_t g);


void slr__print_tables(FILE *stream, action_table_t tbl_action,
    goto_table_t tbl_goto, node_t *C, grammar_t g);

#endif
