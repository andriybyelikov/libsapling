#include "libsapling/dm/avl.h"
#include "cc/lexer/lexer_attributed_edge.h"

struct reserve {
    attributed_edge key;
    node_t *res;
};

static
void reserve_get_ref(attributed_edge *data, void *info)
{
    CAST_USER_INFO(struct reserve *, user, info);

    user->res = &data->node; // node is first field
}



node_t *ae__reserve(node_t *ref, char byte)
{
    struct reserve info = { { .byte = byte }, NULL };
    ae__insert(ref, info.key, ae__cmp_predicate);
    ae__access(E_QT, ref, &info, ae__equ_predicate, reserve_get_ref);
    return info.res;
}

attributed_edge *ae__get_edge(node_t *ref, char byte)
{
    struct reserve info = { { .byte = byte }, NULL };
    ae__access(E_QT, ref, &info, ae__equ_predicate, reserve_get_ref);
    return (attributed_edge *)info.res;
}
