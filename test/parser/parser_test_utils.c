#include <stdlib.h>
#include "libsapling/dm/queue.h"
#include "libsapling/cc/production.h"
#include "libsapling/cc/parser/grammar.h"
#include "libsapling/cc/parser/lr0_item.h"
#include "libsapling/cc/parser/aux/symbol_set.h"
#include "libsapling/cc/parser/aux/production_set.h"
#include "libsapling/cc/parser/aux/set_of_sets_of_lr0_items.h"

IMPLEMENT_TYPED_QUEUE(body_queue, int, int__print)

grammar_t build_test_grammar(void)
{
    /*
     * Example 4.40 from the Dragon Book
     * 
     * E' -> E;
     * E  -> E + T | T;
     * T  -> T * F | F;
     * F  -> ( E ) | id;
     * 
     * 
     * terminals:
     * 0 id
     * 1 +
     * 2 *
     * 3 (
     * 4 )
     * 5 $
     * 
     * nonterminals:
     * 6 E'
     * 7 E
     * 8 T
     * 9 F
     * 
     * productions:
     * 0 [6 -> 7]
     * 1 [7 -> 7 1 8]
     * 2 [7 -> 8]
     * 3 [8 -> 8 2 9]
     * 4 [8 -> 9]
     * 5 [9 -> 3 7 4]
     * 6 [9 -> 0]
     */
    node_t *productions = malloc(sizeof(node_t));
    *productions = NULL;
    node_t *pbodies = malloc(7 * sizeof(node_t));
    for (int i = 0; i < 7; i++)
        pbodies[i] = NULL;
    int pbody_id = 0;
    {
        node_t *pbody = &pbodies[pbody_id];
        body_queue__insert(pbody, 7);
        production_queue__insert(productions, new_production(6, pbody));
        pbody_id++;
    }
    {
        node_t *pbody = &pbodies[pbody_id];
        body_queue__insert(pbody, 7);
        body_queue__insert(pbody, 1);
        body_queue__insert(pbody, 8);
        production_queue__insert(productions, new_production(7, pbody));
        pbody_id++;
    }
    {
        node_t *pbody = &pbodies[pbody_id];
        body_queue__insert(pbody, 8);
        production_queue__insert(productions, new_production(7, pbody));
        pbody_id++;
    }
    {
        node_t *pbody = &pbodies[pbody_id];
        body_queue__insert(pbody, 8);
        body_queue__insert(pbody, 2);
        body_queue__insert(pbody, 9);
        production_queue__insert(productions, new_production(8, pbody));
        pbody_id++;
    }
    {
        node_t *pbody = &pbodies[pbody_id];
        body_queue__insert(pbody, 9);
        production_queue__insert(productions, new_production(8, pbody));
        pbody_id++;
    }
    {
        node_t *pbody = &pbodies[pbody_id];
        body_queue__insert(pbody, 3);
        body_queue__insert(pbody, 7);
        body_queue__insert(pbody, 4);
        production_queue__insert(productions, new_production(9, pbody));
        pbody_id++;
    }
    {
        node_t *pbody = &pbodies[pbody_id];
        body_queue__insert(pbody, 0);
        production_queue__insert(productions, new_production(9, pbody));
        pbody_id++;
    }
    return new_grammar(productions, 6, 10);
}

node_t *build_test_first_sets(void)
{
    /*
     * first( id ) = { id }
     * first( +  ) = { + }
     * first( *  ) = { * }
     * first( (  ) = { ( }
     * first( )  ) = { ) }
     * first( $  ) = { $ }
     * first( E' ) = { (, id }
     * first( E  ) = { (, id }
     * first( T  ) = { (, id }
     * first( F  ) = { (, id }
     * 
     * first(0) = { 0 }
     * first(1) = { 1 }
     * first(2) = { 2 }
     * first(3) = { 3 }
     * first(4) = { 4 }
     * first(5) = { 5 }
     * first(6) = { 3, 0 }
     * first(7) = { 3, 0 }
     * first(8) = { 3, 0 }
     * first(9) = { 3, 0 }
     */
    node_t *first_sets = malloc(10 * sizeof(node_t));
    for (int i = 0; i < 10; i++)
        first_sets[i] = NULL;
    for (int i = 0; i < 6; i++) {
        symbol_queue__insert(&first_sets[i], i);
    }
    for (int i = 6; i < 10; i++) {
        symbol_queue__insert(&first_sets[i], 3);
        symbol_queue__insert(&first_sets[i], 0);
    }
    return first_sets;
}

node_t *build_test_follow_sets(void)
{
    /*
     * follow( E' ) = { $ }
     * follow( E  ) = { $, +, ) }
     * follow( T  ) = { $, +, ), * }
     * follow( F  ) = { $, +, ), * }
     * 
     * follow(6) = { 5 }
     * follow(7) = { 5, 1, 4 }
     * follow(8) = { 5, 1, 4, 2 }
     * follow(9) = { 5, 1, 4, 2 }
     */
    node_t *follow_sets = malloc(4 * sizeof(node_t));
    for (int i = 0; i < 4; i++)
        follow_sets[i] = NULL;
    symbol_queue__insert(&follow_sets[0], 5);
    symbol_queue__insert(&follow_sets[1], 5);
    symbol_queue__insert(&follow_sets[1], 1);
    symbol_queue__insert(&follow_sets[1], 4);
    symbol_queue__insert(&follow_sets[2], 5);
    symbol_queue__insert(&follow_sets[2], 1);
    symbol_queue__insert(&follow_sets[2], 4);
    symbol_queue__insert(&follow_sets[2], 2);
    symbol_queue__insert(&follow_sets[3], 5);
    symbol_queue__insert(&follow_sets[3], 1);
    symbol_queue__insert(&follow_sets[3], 4);
    symbol_queue__insert(&follow_sets[3], 2);
    return follow_sets;
}


struct info_index_productions {
    production_t *parr;
    int i;
};

static
void index_productions_apply(production_t *data, void *info)
{
    CAST_USER_INFO(struct info_index_productions *, user, info);

    user->parr[user->i++] = *data;
}

#define PUT_ITEM(P, C) \
set_of_lr0_items_queue__insert(set_of_items, new_lr0_item(productions[P], C))

node_t *build_test_set_of_sets_of_lr0_items(grammar_t g)
{
    /*
     * I_0  { [6 -> ·7], [7 -> ·7 1 8], [7 -> ·8], [8 -> ·8 2 9], [8 -> ·9],
     *        [9 -> ·3 7 4], [9 -> ·0] }
     * I_1  { [6 -> 7·], [7 -> 7 ·1 8] }
     * I_2  { [7 -> 8·], [8 -> 8 ·2 9] }
     * I_3  { [8 -> 9·] }
     * I_4  { [9 -> 3 ·7 4], [7 -> ·7 1 8], [7 -> ·8], [8 -> ·8 2 9],
     *        [8 -> ·9], [9 -> ·3 7 4], [9 -> ·0] }
     * I_5  { [9 -> 0·] }
     * I_6  { [7 -> 7 1 ·8], [8 -> ·8 2 9], [8 -> ·9], [9 -> ·3 7 4],
     *        [9 -> ·0] }
     * I_7  { [8 -> 8 2 ·9], [9 -> ·3 7 4], [9 -> ·0] }
     * I_8  { [9 -> 3 7 ·4], [7 -> 7 ·1 8] }
     * I_9  { [7 -> 7 1 8·], [8 -> 8 ·2 9] }
     * I_10 { [8 -> 8 2 9·] }
     * I_11 { [9 -> 3 7 4·] }
     * 
     * I_0  { (0, 0), (1, 0), (2, 0), (3, 0), (4, 0), (5, 0), (6, 0) }
     * I_1  { (0, 1), (1, 1) }
     * I_2  { (2, 1), (3, 1) }
     * I_3  { (4, 1) }
     * I_4  { (5, 1), (1, 0), (2, 0), (3, 0), (4, 0), (5, 0), (6, 0) }
     * I_5  { (6, 1) }
     * I_6  { (1, 2), (3, 0), (4, 0), (5, 0), (6, 0) }
     * I_7  { (3, 2), (5, 0), (6, 0) }
     * I_8  { (5, 2), (1, 1) }
     * I_9  { (1, 3), (3, 1) }
     * I_10 { (3, 3) }
     * I_11 { (5, 3) }
     */
    node_t *set_of_set_of_items = malloc(sizeof(node_t));
    *set_of_set_of_items = NULL;

    production_t productions[7];
    struct info_index_productions iip = { productions, 0 };
    production_path__access(U_QT, grammar__productions(g), &iip,
        production_path__predicate_1, index_productions_apply);

    {
        node_t *set_of_items = malloc(sizeof(node_t));
        *set_of_items = NULL;
        PUT_ITEM(0, 0);
        PUT_ITEM(1, 0);
        PUT_ITEM(2, 0);
        PUT_ITEM(3, 0);
        PUT_ITEM(4, 0);
        PUT_ITEM(5, 0);
        PUT_ITEM(6, 0);
        set_of_sets_of_lr0_items_queue__insert(set_of_set_of_items,
            set_of_items);
    }
    {
        node_t *set_of_items = malloc(sizeof(node_t));
        *set_of_items = NULL;
        PUT_ITEM(0, 1);
        PUT_ITEM(1, 1);
        set_of_sets_of_lr0_items_queue__insert(set_of_set_of_items,
            set_of_items);
    }
    {
        node_t *set_of_items = malloc(sizeof(node_t));
        *set_of_items = NULL;
        PUT_ITEM(2, 1);
        PUT_ITEM(3, 1);
        set_of_sets_of_lr0_items_queue__insert(set_of_set_of_items,
            set_of_items);
    }
    {
        node_t *set_of_items = malloc(sizeof(node_t));
        *set_of_items = NULL;
        PUT_ITEM(4, 1);
        set_of_sets_of_lr0_items_queue__insert(set_of_set_of_items,
            set_of_items);
    }
    {
        node_t *set_of_items = malloc(sizeof(node_t));
        *set_of_items = NULL;
        PUT_ITEM(5, 1);
        PUT_ITEM(1, 0);
        PUT_ITEM(2, 0);
        PUT_ITEM(3, 0);
        PUT_ITEM(4, 0);
        PUT_ITEM(5, 0);
        PUT_ITEM(6, 0);
        set_of_sets_of_lr0_items_queue__insert(set_of_set_of_items,
            set_of_items);
    }
    {
        node_t *set_of_items = malloc(sizeof(node_t));
        *set_of_items = NULL;
        PUT_ITEM(6, 1);
        set_of_sets_of_lr0_items_queue__insert(set_of_set_of_items,
            set_of_items);
    }
    {
        node_t *set_of_items = malloc(sizeof(node_t));
        *set_of_items = NULL;
        PUT_ITEM(1, 2);
        PUT_ITEM(3, 0);
        PUT_ITEM(4, 0);
        PUT_ITEM(5, 0);
        PUT_ITEM(6, 0);
        set_of_sets_of_lr0_items_queue__insert(set_of_set_of_items,
            set_of_items);
    }
    {
        node_t *set_of_items = malloc(sizeof(node_t));
        *set_of_items = NULL;
        PUT_ITEM(3, 2);
        PUT_ITEM(5, 0);
        PUT_ITEM(6, 0);
        set_of_sets_of_lr0_items_queue__insert(set_of_set_of_items,
            set_of_items);
    }
    {
        node_t *set_of_items = malloc(sizeof(node_t));
        *set_of_items = NULL;
        PUT_ITEM(5, 2);
        PUT_ITEM(1, 1);
        set_of_sets_of_lr0_items_queue__insert(set_of_set_of_items,
            set_of_items);
    }
    {
        node_t *set_of_items = malloc(sizeof(node_t));
        *set_of_items = NULL;
        PUT_ITEM(1, 3);
        PUT_ITEM(3, 1);
        set_of_sets_of_lr0_items_queue__insert(set_of_set_of_items,
            set_of_items);
    }
    {
        node_t *set_of_items = malloc(sizeof(node_t));
        *set_of_items = NULL;
        PUT_ITEM(3, 3);
        set_of_sets_of_lr0_items_queue__insert(set_of_set_of_items,
            set_of_items);
    }
    {
        node_t *set_of_items = malloc(sizeof(node_t));
        *set_of_items = NULL;
        PUT_ITEM(5, 3);
        set_of_sets_of_lr0_items_queue__insert(set_of_set_of_items,
            set_of_items);
    }

    return set_of_set_of_items;
}


struct info_symbol_set_is_subset_of {
    node_t *b;
    int satisf;
};

static
void symbol_set_is_subset_of_apply(int *data, void *info)
{
    CAST_USER_INFO(struct info_symbol_set_is_subset_of *, user, info);

    user->satisf = 0;
}

static
int symbol_set_is_subset_of_predicate(const int *data, void *info)
{
    CAST_USER_INFO(struct info_symbol_set_is_subset_of *, user, info);

    return !symbol_path__in(user->b, *data);
}

static
int symbol_set__is_subset_of(node_t *a, node_t *b)
{
    struct info_symbol_set_is_subset_of info = { b, 1 };
    symbol_path__access(E_QT, a, &info, symbol_set_is_subset_of_predicate,
        symbol_set_is_subset_of_apply);
    return info.satisf;
}

int symbol_set__compare(node_t *a, node_t *b)
{
    return symbol_set__is_subset_of(a, b) && symbol_set__is_subset_of(b, a);
}
