#include <stdlib.h>
#include "libsapling/cc/parser/lr0_item.h"
#include "libsapling/cc/parser/follow.h"
#include "libsapling/cc/parser/goto.h"
#include "libsapling/cc/parser/action_table.h"
#include "libsapling/cc/parser/aux/production_set.h"
#include "libsapling/cc/parser/aux/set_of_sets_of_lr0_items.h"

struct action_table {
    int action;
    int info; // state id to shift to / production id to reduce to
};


struct info_fill_shifts_1 {
    grammar_t g;
    node_t *I;
    node_t *C;
    action_table_t tbl;
    int i;
};

static
void fill_shifts_1_apply(lr0_item_t *data, void *info)
{
    CAST_USER_INFO(struct info_fill_shifts_1 *, user, info);

    int X = *production__get(lr0_item__production(*data),
        lr0_item__cursor(*data));
    node_t *goto_set = compute_goto(user->I, X, user->g);

    if (set_of_sets_of_lr0_items_path__in(user->C, goto_set)) {
        int i = user->i;
        int j = set_of_sets_of_lr0_items__get_j(user->C, goto_set);
        user->tbl[i * grammar__num_terminals(user->g) + X].action =
            PARSER_ACTION_SHIFT;
        user->tbl[i * grammar__num_terminals(user->g) + X].info = j;
    }
}

static
int fill_shifts_1_predicate(const lr0_item_t *data, void *info)
{
    CAST_USER_INFO(struct info_fill_shifts_1 *, user, info);

    int *sym = production__get(lr0_item__production(*data),
        lr0_item__cursor(*data));
    return sym != NULL && grammar__is_terminal(user->g, *sym);
}


struct info_fill_shifts_0 {
    grammar_t g;
    node_t *C;
    action_table_t tbl;
    int i;
};

static
void fill_shifts_0_apply(node_t **data, void *info)
{
    CAST_USER_INFO(struct info_fill_shifts_0 *, user, info);

    struct info_fill_shifts_1 ifs1 = { user->g, *data, user->C, user->tbl,
        user->i };
    set_of_lr0_items_path__access(U_QT, *data, &ifs1, fill_shifts_1_predicate,
        fill_shifts_1_apply);
    user->i++;
}


struct info_fill_reduces_2 {
    grammar_t g;
    action_table_t tbl;
    int i;
    int production_index;
};

static
void fill_reduces_2_apply(int *data, void *info)
{
    CAST_USER_INFO(struct info_fill_reduces_2 *, user, info);

    int i = user->i;
    user->tbl[i * grammar__num_terminals(user->g) + *data].action
        = PARSER_ACTION_REDUCE;
    user->tbl[i * grammar__num_terminals(user->g) + *data].info
        = user->production_index;
}


struct info_get_production_index {
    production_t production;
    int i;
};

static
void get_production_index_apply(production_t *data, void *info)
{
    return;
}

static
int get_production_index_predicate(const production_t *data, void *info)
{
    CAST_USER_INFO(struct info_get_production_index *, user, info);

    int res = *data == user->production;

    if (!res)
        user->i++;

    return res;
}

static
int get_production_index(grammar_t g, production_t production)
{
    struct info_get_production_index info = { production, 0 };
    production_path__access(E_QT, grammar__productions(g), &info,
        get_production_index_predicate, get_production_index_apply);
    return info.i;
}


struct info_fill_reduces_1 {
    grammar_t g;
    action_table_t tbl;
    int i;
};

static
void fill_reduces_1_apply(lr0_item_t *data, void *info)
{
    CAST_USER_INFO(struct info_fill_reduces_1 *, user, info);

    node_t *follow_set = compute_follow_set(user->g,
        production__id(lr0_item__production(*data)));

    struct info_fill_reduces_2 ifr2 = { user->g, user->tbl, user->i,
        get_production_index(user->g, lr0_item__production(*data)) };
    symbol_path__access(U_QT, follow_set, &ifr2, symbol_path__predicate_1,
        fill_reduces_2_apply);
}

static
int fill_reduces_1_predicate(const lr0_item_t *data, UNUSED void *info)
{
    CAST_USER_INFO(struct info_fill_reduces_1 *, user, info);

    // technically id {num_terminals} is S' aka start
    return production__id(lr0_item__production(*data))
            != grammar__num_terminals(user->g)
        && lr0_item__cursor(*data)
            == production__len(lr0_item__production(*data));
}


struct info_fill_reduces_0 {
    grammar_t g;
    action_table_t tbl;
    int i;
};

static
void fill_reduces_0_apply(node_t **data, void *info)
{
    CAST_USER_INFO(struct info_fill_reduces_0 *, user, info);

    struct info_fill_reduces_1 ifr1 = { user->g, user->tbl, user->i };
    set_of_lr0_items_path__access(U_QT, *data, &ifr1, fill_reduces_1_predicate,
        fill_reduces_1_apply);
    user->i++;
}


struct info_fill_accept_0 {
    grammar_t g;
    action_table_t tbl;
    int i;
    production_t p;
};

static
void fill_accept_0_apply(node_t **data, void *info)
{
    CAST_USER_INFO(struct info_fill_accept_0 *, user, info);

    int i = user->i;
    // X = (grammar__num_terminals(user->g) - 1) is t_eof
    user->tbl[i * grammar__num_terminals(user->g)
        + grammar__num_terminals(user->g) - 1].action = PARSER_ACTION_ACCEPT;
}

static
int fill_accept_0_predicate(const node_t **data, void *info)
{
    CAST_USER_INFO(struct info_fill_accept_0 *, user, info);

    int res = set_of_lr0_items_path__in(*data,
        new_lr0_item(user->p, production__len(user->p)));

    if (!res)
        user->i++;

    return res;
}


action_table_t action_table__build(grammar_t g, node_t *C)
{
    int num_states = set_of_sets_of_lr0_items_path__length(C);
    int num_terminals = grammar__num_terminals(g);

    action_table_t tbl = malloc(
        num_states * num_terminals * sizeof(struct action_table));

    // init table
    for (int i = 0; i < num_states; i++) {
        for (int j = 0; j < num_terminals; j++) {
            tbl[i * num_terminals + j].action = PARSER_ACTION_ERROR;
            tbl[i * num_terminals + j].info = -1;
        }
    }

    // fill shifts
    struct info_fill_shifts_0 ifs0 = { g, C, tbl, 0 };
    set_of_sets_of_lr0_items_path__access(U_QT, C, &ifs0,
        set_of_sets_of_lr0_items_path__predicate_1, fill_shifts_0_apply);

    // fill reduces
    struct info_fill_reduces_0 ifr0 = { g, tbl, 0 };
    set_of_sets_of_lr0_items_path__access(U_QT, C, &ifr0,
        set_of_sets_of_lr0_items_path__predicate_1, fill_reduces_0_apply);

    // fill accept
    struct info_fill_accept_0 ifa0 = { g, tbl, 0,
        production_queue__access(grammar__productions(g)) };
    set_of_sets_of_lr0_items_path__access(E_QT, C, &ifa0,
        fill_accept_0_predicate, fill_accept_0_apply);

    return tbl;
}


int action_table__get_action(action_table_t tbl, grammar_t g, int state,
    int sym)
{
    return tbl[state * grammar__num_terminals(g) + sym].action;
}

int action_table__get_action_info(action_table_t tbl, grammar_t g, int state,
    int sym)
{
    return tbl[state * grammar__num_terminals(g) + sym].info;
}

action_table_t new_action_table(grammar_t g, node_t *C)
{
    int num_states = set_of_sets_of_lr0_items_path__length(C);
    int num_terminals = grammar__num_terminals(g);

    action_table_t tbl = malloc(
        num_states * num_terminals * sizeof(struct action_table));

    // init table
    for (int i = 0; i < num_states; i++) {
        for (int j = 0; j < num_terminals; j++) {
            tbl[i * num_terminals + j].action = PARSER_ACTION_ERROR;
            tbl[i * num_terminals + j].info = -1;
        }
    }

    return tbl;
}

void action_table__set_action(action_table_t tbl, grammar_t g, int state,
    int sym, int action)
{
    tbl[state * grammar__num_terminals(g) + sym].action = action;
}

void action_table__set_action_info(action_table_t tbl, grammar_t g, int state,
    int sym, int action_info)
{
    tbl[state * grammar__num_terminals(g) + sym].info = action_info;
}
