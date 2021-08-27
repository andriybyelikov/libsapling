#include <stdlib.h>
#include "libsapling/dm/queue.h"
#include "libsapling/cc/production.h"
#include "libsapling/cc/parser/grammar.h"
#include "libsapling/cc/parser/aux/symbol_set.h"

IMPLEMENT_TYPED_QUEUE(body_queue, int, int__print)
IMPLEMENT_TYPED_QUEUE(production_queue, production_t, production__print)

grammar_t build_test_grammar(void)
{
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
