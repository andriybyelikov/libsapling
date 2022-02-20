#include "libsapling/cc/parser/defs/symbol_set.h"

struct info_symbol_set_union {
    node_t *first_set;
};

static
void symbol_set_union_apply(int *data, void *info)
{
    CAST_USER_INFO(struct info_symbol_set_union *, user, info);

    symbol_queue__insert(user->first_set, *data);
}

static
int symbol_set_union_predicate(const int *data, void *info)
{
    CAST_USER_INFO(struct info_symbol_set_union *, user, info);

    return !symbol_path__in(user->first_set, *data);
}

void symbol_set__union(node_t *a, node_t *b)
{
    struct info_symbol_set_union issu = { a };
    symbol_path__access(U_QT, b, &issu, symbol_set_union_predicate,
        symbol_set_union_apply);
}
