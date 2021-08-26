#ifndef _LIBSAPLING_GRAMMAR_H_
#define _LIBSAPLING_GRAMMAR_H_

/**
 * @file grammar.h
 * @brief Grammar implementation.
 */

#include "libsapling/dm/graph.h"


typedef struct grammar *grammar_t;


/**
 * @brief Allocates a new grammar on the heap.
 * 
 * @param productions Path of productions.
 * @param num_terminals Number of terminals.
 * @param num_symbols Sum of the number of terminal and nonterminal symbols.
 * 
 * @returns pointer to the allocated grammar.
 */
grammar_t new_grammar(node_t *productions, int num_terminals, int num_symbols);

/**
 * @brief Frees the memory previously allocated on the heap for a grammar with
 * @ref new_grammar.
 */
void delete_grammar(grammar_t g);

/**
 * @brief Returns the grammar's path of productions.
 * @returns the grammar's path of productions.
 */
node_t *grammar__productions(grammar_t g);

/**
 * @brief Returns the number of terminals in the grammar.
 * @returns the number of terminals in the grammar.
 */
int grammar__num_terminals(grammar_t g);

/**
 * @brief Returns the number of nonterminals in the grammar.
 * @returns the number of nonterminals in the grammar.
 */
int grammar__num_nonterminals(grammar_t g);

/**
 * @brief Returns the sum of the number of terminals and nonterminals in the
 * grammar.
 * @returns the sum of the number of terminals and nonterminals in the grammar.
 */
int grammar__num_symbols(grammar_t g);

/**
 * @brief Returns whether the symbol is a terminal in the grammar.
 * @returns whether the symbol is a terminal in the grammar.
 */
int grammar__is_terminal(grammar_t g, int symbol);

#endif
