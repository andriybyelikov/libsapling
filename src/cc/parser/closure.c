#include "libsapling/cc/parser/grammar.h"
#include "libsapling/cc/parser/defs/production_set.h"
#include "libsapling/cc/parser/defs/set_of_lr0_items.h"

struct info_closure_1 {
    int *repeat;
    node_t *I;
    int B;
};

static
void closure_1_apply(production_t *data, void *info)
{
    CAST_USER_INFO(struct info_closure_1 *, user, info);

    // add [B->.γ] to J
    lr0_item_t item = new_lr0_item(*data, 0);
    set_of_lr0_items_queue__insert(user->I, item);

    // if repeat not set then set repeat
    if (!*(user->repeat)) {
        *(user->repeat) = 1;
    }
}

static
int closure_1_predicate(const production_t *data, void *info)
{
    CAST_USER_INFO(struct info_closure_1 *, user, info);

    // id == B and [B->.γ] is not in J
    if (production__id(*data) != user->B)
        return 0;

    lr0_item_t item = new_lr0_item(*data, 0);
    return !set_of_lr0_items_path__in(user->I, item);
}

struct info_closure_0 {
    int *repeat;
    node_t *I;
    grammar_t g;
};

static
void closure_0_apply(lr0_item_t *data, void *info)
{
    CAST_USER_INFO(struct info_closure_0 *, user, info);

    struct info_closure_1 ic1 = { user->repeat, user->I,
        *production__get(lr0_item__production(*data),
        lr0_item__cursor(*data)) };
    production_path__access(U_QT, grammar__productions(user->g), &ic1,
        closure_1_predicate, closure_1_apply);
}

static
int closure_0_predicate(const lr0_item_t *data, void *info)
{
    CAST_USER_INFO(struct info_closure_0 *, user, info);

    // cursor points to nonterminal
    int *sym = production__get(lr0_item__production(*data),
        lr0_item__cursor(*data));
    return sym != NULL && !grammar__is_terminal(user->g, *sym);
}

// I: set of items
void closure(node_t *I, grammar_t g)
{
    int repeat = 1;
    while (repeat) {
        repeat = 0;
        struct info_closure_0 ic0 = { &repeat, I, g };
        set_of_lr0_items_path__access(U_QT, I, &ic0,
            closure_0_predicate, closure_0_apply);
    }
}
