#ifndef _LIBSAPLING_PARSER_AUX_SET_OF_LR0_ITEMS_H_
#define _LIBSAPLING_PARSER_AUX_SET_OF_LR0_ITEMS_H_

#include "libsapling/dm/path.h"
#include "libsapling/dm/queue.h"
#include "libsapling/cc/parser/lr0_item.h"

IMPLEMENT_TYPED_PATH(set_of_lr0_items_path, lr0_item_t, lr0_item__print,
    lr0_item__equals)
IMPLEMENT_TYPED_QUEUE(set_of_lr0_items_queue, lr0_item_t, lr0_item__print)

int set_of_lr0_items__equals(const void *a, const void *b);
void set_of_lr0_items__print(FILE *stream, const void *data);

#endif
