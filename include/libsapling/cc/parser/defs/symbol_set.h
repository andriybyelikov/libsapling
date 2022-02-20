#ifndef _LIBSAPLING_PARSER_AUX_SYMBOL_SET_H_
#define _LIBSAPLING_PARSER_AUX_SYMBOL_SET_H_

#include "libsapling/dm/path.h"
#include "libsapling/dm/queue.h"

IMPLEMENT_TYPED_PATH(symbol_path, int, int__print, int__equals)
IMPLEMENT_TYPED_QUEUE(symbol_queue, int, int__print)

void symbol_set__union(node_t *a, node_t *b);

#endif
