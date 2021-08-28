#ifndef _LIBSAPLING_PARSER_CLOSURE_H_
#define _LIBSAPLING_PARSER_CLOSURE_H_

/**
 * @file closure.h
 * @brief Implementation of the \e closure function for Simple LR.
 */

#include "libsapling/dm/graph.h"
#include "libsapling/cc/parser/grammar.h"

void closure(node_t *I, grammar_t g);

#endif
