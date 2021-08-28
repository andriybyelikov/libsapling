#ifndef _LIBSAPLING_PARSER_AUX_SET_OF_SETS_OF_LR0_ITEMS_H_
#define _LIBSAPLING_PARSER_AUX_SET_OF_SETS_OF_LR0_ITEMS_H_

#include "libsapling/dm/path.h"
#include "libsapling/dm/queue.h"
#include "libsapling/cc/parser/aux/set_of_lr0_items.h"

IMPLEMENT_TYPED_PATH(set_of_sets_of_lr0_items_path, pnode_t,
    set_of_lr0_items__print, set_of_lr0_items__equals)
IMPLEMENT_TYPED_QUEUE(set_of_sets_of_lr0_items_queue, pnode_t,
    set_of_lr0_items__print)

int set_of_sets_of_lr0_items__equals(const void *a, const void *b);

int set_of_sets_of_lr0_items__get_j(node_t *C, node_t *I);

#endif
