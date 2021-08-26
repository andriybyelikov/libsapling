#include <assert.h>
#include "libsapling/dm/queue.h"
#include "libsapling/cc/production.h"

IMPLEMENT_TYPED_QUEUE(body, int, int__print)

int main(void)
{
    // build production
    node_t body = NULL;
    body__insert(&body, 1);
    body__insert(&body, 2);
    body__insert(&body, 3);
    production_t p = new_production(0, &body);

    assert(production__id(p) == 0);
    assert(production__len(p) == 3);
    assert(*production__get(p, 0) == 1);
    assert(*production__get(p, 1) == 2);
    assert(*production__get(p, 2) == 3);

    // production__print(stderr, &p);
}
