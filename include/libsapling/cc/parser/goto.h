#ifndef _LIBSAPLING_PARSER_GOTO_H_
#define _LIBSAPLING_PARSER_GOTO_H_

/**
 * @file goto.h
 * @brief Implementation of the \e goto function for Simple LR.
 */

#include "libsapling/dm/graph.h"
#include "libsapling/cc/parser/grammar.h"

node_t *compute_goto(node_t *I, int X, grammar_t g);

#endif
