#include <stdlib.h>
#include "libsapling/cc/parser/grammar.h"
#include "libsapling/cc/parser/closure.h"
#include "libsapling/cc/parser/goto.h"
#include "libsapling/cc/parser/aux/production_set.h"
#include "libsapling/cc/parser/aux/set_of_sets_of_lr0_items.h"

struct info_build_set_of_sets_of_lr0_items {
    node_t *C;
    grammar_t g;
};

static
void build_set_of_sets_of_lr0_items_apply(pnode_t *data, void *info)
{
    CAST_USER_INFO(struct info_build_set_of_sets_of_lr0_items *, user, info);

    // for each grammar symbol
    for (int X = 0; X < grammar__num_symbols(user->g); X++) {
        node_t *goto_set = compute_goto(*data, X, user->g);

        // if goto(I,X) is not empty and not in C then add GOTO(I,X) to C
        if (*goto_set != NULL &&
            !set_of_sets_of_lr0_items_path__in(user->C, goto_set)) {
            set_of_sets_of_lr0_items_queue__insert(user->C, goto_set);
        }
    }
}

node_t *slr__build_set_of_sets_of_lr0_items(grammar_t g)
{
    // C is set of sets of items
    node_t *C = malloc(sizeof(node_t));
    *C = NULL;

    // production 0 should be [S'->S]
    lr0_item_t initial_item = new_lr0_item(
        production_queue__access(grammar__productions(g)), 0);

    node_t *initial_set_of_items = malloc(sizeof(node_t));
    *initial_set_of_items = NULL;
    set_of_lr0_items_queue__insert(initial_set_of_items, initial_item);

    // closure({[S'->·S]})
    closure(initial_set_of_items, g);

    set_of_sets_of_lr0_items_queue__insert(C, initial_set_of_items);

    int repeat = 1;
    while (repeat) {
        repeat = 0;
        struct info_build_set_of_sets_of_lr0_items info = { C, g };
        set_of_sets_of_lr0_items_path__access(U_QT, C, &info,
            set_of_sets_of_lr0_items_path__predicate_1,
            build_set_of_sets_of_lr0_items_apply);
    }

    return C;
}
