#ifndef _LIBSAPLING_PARSER_FIRST_H_
#define _LIBSAPLING_PARSER_FIRST_H_

/**
 * @file first.h
 * @brief Implementation of the computation of the first set
 */

#include "libsapling/dm/graph.h"
#include "libsapling/cc/parser/grammar.h"
#include "libsapling/cc/parser/aux/symbol_set.h"

/**
 * @brief Computes the first set of a grammar symbol.
 * 
 * @returns path (symbol set) with all the terminal symbols that are part of
 * the first set of the specified symbol.
 * 
 * @warning This implementation does not yet handle empty strings (ε, λ, Λ).
 */
node_t *compute_first_set(grammar_t g, int symbol);

#endif
