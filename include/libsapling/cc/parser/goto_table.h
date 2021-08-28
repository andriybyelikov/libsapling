#ifndef _LIBSAPLING_PARSER_GOTO_TABLE_H_
#define _LIBSAPLING_PARSER_GOTO_TABLE_H_

/**
 * @file goto_table.h
 * @brief Parser \e goto table implementation.
 */

#include "libsapling/dm/graph.h"
#include "libsapling/cc/parser/grammar.h"


typedef struct goto_table *goto_table_t;

/**
 * @brief Builds the \e goto table.
 * 
 * @param g A grammar
 * @param C A set of sets of LR(0) item
 */
goto_table_t goto_table__build(grammar_t g, node_t *C);

/**
 * @brief Returns the state to go to after reducing a production.
 * 
 * @param tbl The \e goto table
 * @param g The grammar that was used to build the table
 * @param state The state
 * @param sym The nonterminal symbol
 */
int goto_table__get_state(goto_table_t tbl, grammar_t g, int state, int sym);

/**
 * @brief Allocates memory for a \e goto table on the heap.
 * 
 * @param g A grammar
 * @param C A set of sets of LR(0) item
 * 
 * @attention This method was written for testing purposes.
 */
goto_table_t new_goto_table(grammar_t g, node_t *C);

/**
 * @brief Sets the state to go to after reducing a production.
 * 
 * @param tbl The \e goto table
 * @param g The grammar that was used to build the table
 * @param state The state
 * @param sym The nonterminal symbol
 * 
 * @attention This method was written for testing purposes.
 */
void goto_table__set_state(goto_table_t tbl, grammar_t g, int state, int sym,
    int go_to_state);

#endif
