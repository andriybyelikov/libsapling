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
