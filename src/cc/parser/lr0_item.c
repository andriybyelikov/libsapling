#include <stdlib.h>
#include "libsapling/cc/parser/lr0_item.h"

struct lr0_item {
    production_t production;
    int cursor;
};

lr0_item_t new_lr0_item(production_t production, int cursor)
{
    lr0_item_t item = malloc(sizeof(struct lr0_item));
    item->production = production;
    item->cursor = cursor;
    return item;
}

void delete_lr0_item(lr0_item_t item)
{
    free(item);
}

production_t lr0_item__production(lr0_item_t item)
{
    return item->production;
}

int lr0_item__cursor(lr0_item_t item)
{
    return item->cursor;
}

int lr0_item__equals(const void *a, const void *b)
{
    lr0_item_t x = *(lr0_item_t *)a;
    lr0_item_t y = *(lr0_item_t *)b;

    return x->production == y->production && x->cursor == y->cursor;
}

void lr0_item__print(FILE *stream, const void *data)
{
    lr0_item_t item = *(lr0_item_t *)data;
    production_t prod = lr0_item__production(item);

    fprintf(stream, "[%d -> ", production__id(prod));
    int i;
    for (i = 0; i < production__len(prod) - 1; i++) {
        if (lr0_item__cursor(item) == i)
            fprintf(stream, "·");
        fprintf(stream, "%d ", *production__get(prod, i));
    }
    if (lr0_item__cursor(item) == i)
        fprintf(stream, "·");
    fprintf(stream, "%d", *production__get(prod, i));
    if (lr0_item__cursor(item) == i + 1)
        fprintf(stream, "·");
    fprintf(stream, "]");
}
