#include <stdlib.h>
#include "libsapling/cc/parser/goto.h"
#include "libsapling/cc/parser/goto_table.h"
#include "libsapling/cc/parser/aux/set_of_sets_of_lr0_items.h"

struct goto_table {
    int state; // state id to go to on having reduced a production
};


struct info_fill_goto_0 {
    grammar_t g;
    goto_table_t tbl;
    int i;
    node_t *C;
};

static
void fill_goto_0_apply(node_t **data, void *info)
{
    CAST_USER_INFO(struct info_fill_goto_0 *, user, info);

    node_t *I = *data;
    int num_nonterminals = grammar__num_nonterminals(user->g);

    for (int X = grammar__num_terminals(user->g) + 1;
            X < grammar__num_symbols(user->g); X++) {
        node_t *goto_set = compute_goto(I, X, user->g);

        if (set_of_sets_of_lr0_items_path__in(user->C, goto_set)) {
            int i = user->i;
            int j = set_of_sets_of_lr0_items__get_j(user->C, goto_set);
            user->tbl[i * num_nonterminals
                + (X - grammar__num_terminals(user->g))].state = j;
        }
    }

    user->i++;
}

goto_table_t goto_table__build(grammar_t g, node_t *C)
{
    int num_states = set_of_sets_of_lr0_items_path__length(C);
    int num_nonterminals = grammar__num_nonterminals(g);

    goto_table_t tbl = malloc(
        num_states * num_nonterminals * sizeof(struct goto_table));

    // init table
    for (int i = 0; i < num_states; i++) {
        for (int j = 0; j < num_nonterminals; j++) {
            tbl[i * num_nonterminals + j].state = -1;
        }
    }

    // fill goto
    struct info_fill_goto_0 ifg0 = { g, tbl, 0, C };
    set_of_sets_of_lr0_items_path__access(U_QT, C, &ifg0,
        set_of_sets_of_lr0_items_path__predicate_1, fill_goto_0_apply);

    return tbl;
}


int goto_table__get_state(goto_table_t tbl, grammar_t g, int state, int sym)
{
    return tbl[state * grammar__num_nonterminals(g)
        + (sym - grammar__num_terminals(g))].state;
}


goto_table_t new_goto_table(grammar_t g, node_t *C)
{
    int num_states = set_of_sets_of_lr0_items_path__length(C);
    int num_nonterminals = grammar__num_nonterminals(g);

    goto_table_t tbl = malloc(
        num_states * num_nonterminals * sizeof(struct goto_table));

    // init table
    for (int i = 0; i < num_states; i++) {
        for (int j = 0; j < num_nonterminals; j++) {
            tbl[i * num_nonterminals + j].state = -1;
        }
    }

    return tbl;
}

void goto_table__set_state(goto_table_t tbl, grammar_t g, int state, int sym,
    int go_to_state)
{
    tbl[state * grammar__num_nonterminals(g)
        + (sym - grammar__num_terminals(g))].state = go_to_state;
}
