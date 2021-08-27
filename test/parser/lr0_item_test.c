#include <assert.h>
#include "libsapling/dm/queue.h"
#include "libsapling/cc/parser/lr0_item.h"

IMPLEMENT_TYPED_QUEUE(body, int, int__print)

int main(void)
{
    // build LR(0) item
    node_t body = NULL;
    body__insert(&body, 1);
    body__insert(&body, 2);
    body__insert(&body, 3);
    production_t p = new_production(0, &body);
    lr0_item_t item = new_lr0_item(p, 1);

    assert(lr0_item__production(item) == p);
    assert(lr0_item__cursor(item) == 1);

    // build another LR(0) item with the same production but different cursor
    // position and assert that they are different LR(0) items
    lr0_item_t item2 = new_lr0_item(p, 0);
    assert(!lr0_item__equals(&item, &item2));

    // build another LR(0) item with the same cursor position but different
    // production and assert that they are different LR(0) items
    node_t body2 = NULL;
    body__insert(&body2, 4);
    body__insert(&body2, 2);
    production_t p2 = new_production(0, &body2);
    lr0_item_t item3 = new_lr0_item(p2, 1);
    assert(!lr0_item__equals(&item, &item3));

    delete_lr0_item(item);
    delete_lr0_item(item2);
    delete_lr0_item(item3);
}
