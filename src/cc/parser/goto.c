#include <stdlib.h>
#include "libsapling/cc/parser/grammar.h"
#include "libsapling/cc/parser/closure.h"
#include "libsapling/cc/parser/defs/set_of_lr0_items.h"
#include "libsapling/cc/parser/defs/production_set.h"

struct info_compute_goto_0 {
    node_t *goto_set;
    int X;
};

static
void compute_goto_0_apply(lr0_item_t *data, void *info)
{
    CAST_USER_INFO(struct info_compute_goto_0 *, user, info);

    set_of_lr0_items_queue__insert(user->goto_set,
        new_lr0_item(
            lr0_item__production(*data), lr0_item__cursor(*data) + 1));
}

static
int compute_goto_0_predicate(const lr0_item_t *data, void *info)
{
    CAST_USER_INFO(struct info_compute_goto_0 *, user, info);

    int *sym = production__get(lr0_item__production(*data),
        lr0_item__cursor(*data));

    return (lr0_item__cursor(*data) + 1)
            <= production__len(lr0_item__production(*data))
        && sym != NULL && *sym == user->X;
}

node_t *compute_goto(node_t *I, int X, grammar_t g)
{
    node_t *goto_set = malloc(sizeof(node_t));
    *goto_set = NULL;

    // compute the set of all items [A->αX·β] such that [A->α·Xβ] is in I
    struct info_compute_goto_0 info = { goto_set, X };
    set_of_lr0_items_path__access(U_QT, I, &info, compute_goto_0_predicate,
        compute_goto_0_apply);

    // return the closure of this set
    closure(goto_set, g);
    return goto_set;
}
