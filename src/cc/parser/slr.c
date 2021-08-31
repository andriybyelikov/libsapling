#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "libsapling/cc/parser/slr.h"
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


void slr__print_tables(FILE *stream, action_table_t tbl_action,
    goto_table_t tbl_goto, node_t *C, grammar_t g)
{
    int num_states = set_of_sets_of_lr0_items_path__length(C);
    int num_symbols = grammar__num_symbols(g);
    int num_terminals = grammar__num_terminals(g);
    int num_nonterminals = num_symbols - grammar__num_terminals(g);

    int cols_state = (int)fmax((int)(log10(num_states - 1) + 1),
        strlen("state"));
    int cols_symbol = (int)fmax((int)(log10(num_symbols - 1) + 1) + 1, 5);
    int cols_terminals = cols_symbol * num_terminals;
    int cols_terminals_pre_action = (cols_terminals - strlen("ACTION")) / 2;
    int cols_terminals_post_action = (cols_terminals - strlen("ACTION")) / 2;
    int cols_nonterminals = cols_symbol * (num_nonterminals - 1);
    int cols_nonterminals_pre_action = (cols_nonterminals - strlen("GOTO"))
        / 2;
    int cols_nonterminals_post_action = (cols_nonterminals - strlen("GOTO"))
        / 2;


    fprintf(stream, "      ");
    for (int i = 0; i < cols_terminals_pre_action; i++)
        fprintf(stream, " ");
    fprintf(stream, "ACTION");
    for (int i = 0; i < cols_terminals_post_action; i++)
        fprintf(stream, " ");
    for (int i = 0; i < cols_nonterminals_pre_action; i++)
        fprintf(stream, " ");
    fprintf(stream, "GOTO");
    for (int i = 0; i < cols_nonterminals_post_action; i++)
        fprintf(stream, " ");
    fprintf(stream, "\n");

    fprintf(stream, "      ");
    for (int i = 0; i < cols_terminals; i++)
        fprintf(stream, "-");
    fprintf(stream, " ");
    for (int i = 0; i < cols_nonterminals; i++)
        fprintf(stream, "-");
    fprintf(stream, "\n");

    fprintf(stream, "state ");
    for (int i = 0; i < num_terminals; i++)
        fprintf(stream, "%*d", cols_symbol, i);
    fprintf(stream, " ");
    for (int i = num_terminals + 1; i < num_symbols; i++)
        fprintf(stream, "%*d", cols_symbol, i);
    fprintf(stream, "\n");

    int su_len = cols_symbol - 1;
    char su[8];
    for (int i = 0; i < su_len; i++)
        su[i] = '-';
    su[su_len] = 0;

    fprintf(stream, "----- ");
    for (int i = 0; i < num_terminals; i++)
        fprintf(stream, "%*s", cols_symbol, su);
    fprintf(stream, " ");
    for (int i = num_terminals + 1; i < num_symbols; i++)
        fprintf(stream, "%*s", cols_symbol, su);
    fprintf(stream, "\n");


    for (int i = 0; i < num_states; i++) {
        fprintf(stream, "%*d  ", cols_state, i);
        for (int j = 0; j < num_terminals; j++) {
            char buf[64];
            buf[0] = 0;
            switch (action_table__get_action(tbl_action, g, i, j)) {
            case PARSER_ACTION_ERROR:
                fprintf(stream, "%*s ", cols_symbol - 1, "-");
                break;
            case PARSER_ACTION_SHIFT:
                buf[sprintf(buf, "s%d",
                    action_table__get_action_info(tbl_action, g, i, j))] = 0;
                fprintf(stream, "%*s ", cols_symbol - 1, buf);
                break;
            case PARSER_ACTION_REDUCE:
                buf[sprintf(buf, "r%d",
                    action_table__get_action_info(tbl_action, g, i, j))] = 0;
                fprintf(stream, "%*s ", cols_symbol - 1, buf);
                break;
            case PARSER_ACTION_ACCEPT:
                fprintf(stream, "%*s ", cols_symbol - 1, "acc");
                break;
            }
        }
        for (int j = num_terminals + 1; j < num_symbols; j++) {
            int state = goto_table__get_state(tbl_goto, g, i, j);
            if (state == -1) {
                fprintf(stream, "%*s", cols_symbol, "-");
            } else {
                fprintf(stream, "%*d", cols_symbol, state);
            }
        }
        fprintf(stream, "\n");
    }
}
