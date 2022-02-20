#include "libsapling/cc/parser/defs/set_of_sets_of_lr0_items.h"

struct info_is_subset_of {
    node_t *b;
    int satisf;
};

static
void is_subset_of_apply(UNUSED node_t **data, void *info)
{
    CAST_USER_INFO(struct info_is_subset_of *, user, info);

    user->satisf = 0;
}

static
int is_subset_of_predicate(const node_t **data, void *info)
{
    CAST_USER_INFO(struct info_is_subset_of *, user, info);

    return !set_of_sets_of_lr0_items_path__in(user->b, *data);
}

static
int set_of_sets_of_lr0_items_is_subset_of(node_t *a, node_t *b)
{
    struct info_is_subset_of info = { b, 1 };
    set_of_sets_of_lr0_items_path__access(E_QT, a, &info,
        is_subset_of_predicate, is_subset_of_apply);
    return info.satisf;
}

/*
 * Remember! For two sets to be equal A ⊆ B ∧ B ⊆ A must be satisfied!
 */

int set_of_sets_of_lr0_items__equals(const void *a, const void *b)
{
    node_t *x = *(node_t **)a;
    node_t *y = *(node_t **)b;

    return set_of_sets_of_lr0_items_is_subset_of(x, y)
        && set_of_sets_of_lr0_items_is_subset_of(y, x);
}


struct info_get_j {
    node_t *I;
    int j;
};

static
void get_j_apply(node_t **data, void *info)
{
    return;
}

static
int get_j_predicate(const node_t **data, void *info)
{
    CAST_USER_INFO(struct info_get_j *, user, info);

    node_t *a = *data;
    node_t *b = user->I;
    int res = set_of_lr0_items__equals(&a, &b);

    if (!res)
        user->j++;

    return res;
}

int set_of_sets_of_lr0_items__get_j(node_t *C, node_t *I)
{
    struct info_get_j info = { I, 0 };
    set_of_sets_of_lr0_items_path__access(E_QT, C, &info, get_j_predicate,
        get_j_apply);
    return info.j;
}
