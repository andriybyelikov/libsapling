#ifndef _LIBSAPLING_PARSER_ACTION_TABLE_H_
#define _LIBSAPLING_PARSER_ACTION_TABLE_H_

/**
 * @file action_table.h
 * @brief Parser \e action table implementation.
 */

#include "libsapling/dm/graph.h"
#include "libsapling/cc/parser/grammar.h"


typedef struct action_table *action_table_t;


enum {
    PARSER_ACTION_ERROR,
    PARSER_ACTION_SHIFT,
    PARSER_ACTION_REDUCE,
    PARSER_ACTION_ACCEPT
};

/**
 * @brief Builds the \e action table.
 * 
 * @param g A grammar
 * @param C A set of sets of LR(0) item
 */
action_table_t action_table__build(grammar_t g, node_t *C);

/**
 * @brief Returns the parser action to perform on a particular state and
 * terminal.
 * 
 * @param tbl The \e action table
 * @param g The grammar that was used to build the table
 * @param state The state
 * @param sym The terminal symbol
 */
int action_table__get_action(action_table_t tbl, grammar_t g, int state,
    int sym);

/**
 * @brief Returns the parser action information on a particular state and
 * terminal.
 * 
 * @param tbl The \e action table
 * @param g The grammar that was used to build the table
 * @param state The state
 * @param sym The terminal symbol
 */
int action_table__get_action_info(action_table_t tbl, grammar_t g, int state,
    int sym);

/**
 * @brief Allocates memory for an \e action table on the heap.
 * 
 * @param g A grammar
 * @param C A set of sets of LR(0) item
 * 
 * @attention This method was written for testing purposes.
 */
action_table_t new_action_table(grammar_t g, node_t *C);

/**
 * @brief Sets the parser action to perform on a particular state and terminal.
 * 
 * @param tbl The \e action table
 * @param g The grammar
 * @param state The state
 * @param sym The terminal symbol
 * @param action The action
 * 
 * @attention This method was written for testing purposes.
 */
void action_table__set_action(action_table_t tbl, grammar_t g, int state,
    int sym, int action);

/**
 * @brief Sets the parser action information on a particular state and
 * terminal.
 * 
 * @param tbl The \e action table
 * @param g The grammar
 * @param state The state
 * @param sym The terminal symbol
 * @param action_info The action information
 * 
 * @attention This method was written for testing purposes.
 */
void action_table__set_action_info(action_table_t tbl, grammar_t g, int state,
    int sym, int action_info);

#endif
