#include "libsapling/cc/parser/aux/set_of_lr0_items.h"

struct info_is_subset_of {
    node_t *b;
    int satisf;
};

static
void is_subset_of_apply(UNUSED lr0_item_t *data, void *info)
{
    CAST_USER_INFO(struct info_is_subset_of *, user, info);

    user->satisf = 0;
}

static
int is_subset_of_predicate(const lr0_item_t *data, void *info)
{
    CAST_USER_INFO(struct info_is_subset_of *, user, info);

    return !set_of_lr0_items_path__in(user->b, *data);
}

static
int set_of_lr0_items_is_subset_of(node_t *a, node_t *b)
{
    struct info_is_subset_of info = { b, 1 };
    set_of_lr0_items_path__access(E_QT, a, &info, is_subset_of_predicate,
        is_subset_of_apply);
    return info.satisf;
}

/*
 * Remember! For two sets to be equal A ⊆ B ∧ B ⊆ A must be satisfied!
 */

int set_of_lr0_items__equals(const void *a, const void *b)
{
    pnode_t x = *(pnode_t *)a;
    pnode_t y = *(pnode_t *)b;

    return set_of_lr0_items_is_subset_of(x, y)
        && set_of_lr0_items_is_subset_of(y, x);
}

void set_of_lr0_items__print(FILE *stream, const void *data)
{
    pnode_t pnode = *(pnode_t *)data;
    set_of_lr0_items_path__print(stream, pnode);
}
