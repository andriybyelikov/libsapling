#ifndef _LIBSAPLING_PARSER_FOLLOW_H_
#define _LIBSAPLING_PARSER_FOLLOW_H_

/**
 * @file follow.h
 * @brief Implementation of the computation of the follow set
 */

#include "libsapling/dm/graph.h"
#include "libsapling/cc/parser/grammar.h"
#include "libsapling/cc/parser/aux/symbol_set.h"

/**
 * @brief Computes the follow set of a nonterminal grammar symbol.
 * 
 * @returns path (symbol set) with all the terminal symbols that are part of
 * the follow set of the specified nonterminal symbol.
 * 
 * @warning This implementation does not yet handle empty strings (ε, λ, Λ).
 */
node_t *compute_follow_set(grammar_t g, int symbol);

#endif
