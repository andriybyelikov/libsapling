#include <stdlib.h>
#include "libsapling/cc/parser/grammar.h"
#include "libsapling/cc/parser/aux/symbol_set.h"
#include "libsapling/cc/parser/aux/production_set.h"

node_t *compute_first_set(grammar_t g, int symbol);

struct info_compute_first_set {
    node_t *first_set;
    int id;
    grammar_t g;
};

static
void compute_first_set_apply(production_t *data, void *info)
{
    CAST_USER_INFO(struct info_compute_first_set *, user, info);

    symbol_set__union(user->first_set,
        compute_first_set(user->g, *production__get(*data, 0)));
}

static
int compute_first_set_predicate(const production_t *data, void *info)
{
    CAST_USER_INFO(struct info_compute_first_set *, user, info);

    return production__id(*data) == user->id
        && *production__get(*data, 0) != user->id;
}

node_t *compute_first_set(grammar_t g, int symbol)
{
    node_t *first_set = malloc(sizeof(node_t));
    *first_set = NULL;

    if (grammar__is_terminal(g, symbol)) { 
        symbol_queue__insert(first_set, symbol);
    } else {
        struct info_compute_first_set icfs0 = { first_set, symbol, g };
        production_path__access(U_QT, grammar__productions(g), &icfs0,
            compute_first_set_predicate, compute_first_set_apply);
    }

    return first_set;
}
