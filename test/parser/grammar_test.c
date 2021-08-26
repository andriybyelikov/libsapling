#include <assert.h>
#include "libsapling/dm/queue.h"
#include "libsapling/cc/parser/grammar.h"
#include "libsapling/cc/production.h"

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

IMPLEMENT_TYPED_QUEUE(body_queue, int, int__print)
IMPLEMENT_TYPED_QUEUE(production_queue, production_t, production__print)

int main(void)
{
    // build grammar
    node_t productions = NULL;
    node_t pbodies[7]; for (int i = 0; i < 7; i++) pbodies[i] = NULL;
    int pbody_id = 0;
    {
        node_t *pbody = &pbodies[pbody_id];
        body_queue__insert(pbody, 7);
        production_queue__insert(&productions, new_production(6, pbody));
        pbody_id++;
    }
    {
        node_t *pbody = &pbodies[pbody_id];
        body_queue__insert(pbody, 7);
        body_queue__insert(pbody, 1);
        body_queue__insert(pbody, 8);
        production_queue__insert(&productions, new_production(7, pbody));
        pbody_id++;
    }
    {
        node_t *pbody = &pbodies[pbody_id];
        body_queue__insert(pbody, 8);
        production_queue__insert(&productions, new_production(7, pbody));
        pbody_id++;
    }
    {
        node_t *pbody = &pbodies[pbody_id];
        body_queue__insert(pbody, 8);
        body_queue__insert(pbody, 2);
        body_queue__insert(pbody, 9);
        production_queue__insert(&productions, new_production(8, pbody));
        pbody_id++;
    }
    {
        node_t *pbody = &pbodies[pbody_id];
        body_queue__insert(pbody, 9);
        production_queue__insert(&productions, new_production(8, pbody));
        pbody_id++;
    }
    {
        node_t *pbody = &pbodies[pbody_id];
        body_queue__insert(pbody, 3);
        body_queue__insert(pbody, 7);
        body_queue__insert(pbody, 4);
        production_queue__insert(&productions, new_production(9, pbody));
        pbody_id++;
    }
    {
        node_t *pbody = &pbodies[pbody_id];
        body_queue__insert(pbody, 0);
        production_queue__insert(&productions, new_production(9, pbody));
        pbody_id++;
    }
    grammar_t g = new_grammar(&productions, 6, 10);

    assert(grammar__num_terminals(g) == 6);
    assert(grammar__num_nonterminals(g) == 4);
    assert(grammar__num_symbols(g) == 10);
    for (int i = 0; i < 6; i++) assert(grammar__is_terminal(g, i));
    for (int i = 6; i < 10; i++) assert(!grammar__is_terminal(g, i));
    delete_grammar(g);
}
