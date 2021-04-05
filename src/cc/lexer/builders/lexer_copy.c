#include "libsapling/dm/avl.h"
#include "libsapling/cc/lexer.h"
#include "cc/lexer/lexer_edge_storage.h"
#include "cc/lexer/lexer_attributed_edges.h"
#include "cc/lexer/lexer_insert_state.h"

typedef struct {
    node_t key;
    node_t value;
} node_node_mapping;

static
int nnm_cmp(const void *a, const void *b)
{
    return ((node_node_mapping *)a)->key <= ((node_node_mapping *)b)->key;
}

static
int nnm_equ(const void *a, const void *b)
{
    return ((node_node_mapping *)a)->key == ((node_node_mapping *)b)->key;
}

static
void nnm_fpf(FILE *stream, const void *data)
{
    node_node_mapping *m = (node_node_mapping *)data;
    fprintf(stream, "(%p, %p)", m->key, m->value);
}

IMPLEMENT_TYPED_AVL(nnm, node_node_mapping, nnm_cmp, nnm_equ, nnm_fpf)

static
void nnm__get_value_apply(node_node_mapping *data, void *info)
{
    struct info_insert *ii = info;
    node_node_mapping *user = ii->info;
    user->value = data->value;
}

static
node_t nnm__get_value(node_t *ref, node_t key)
{
    node_node_mapping m = { .key = key };
    nnm__access(E_QT, ref, &m, nnm__equ_predicate, nnm__get_value_apply);
    return m.value;
}


struct copy_attributed_edges {
    node_t *map;
    node_t copy;
};

static
void copy_attributed_edges(attributed_edge *data, void *info)
{
    struct info_insert *ii = info;
    struct copy_attributed_edges *user = ii->info;
    node_t *map = user->map;
    node_t copy = user->copy;

    // insert edge in copy attributed edges changing the original reference
    // with the copy reference
    *ae__reserve(&copy->attributed_edges, data->byte)
        = nnm__get_value(map, data->node);
}

static
void copy_edges(node_node_mapping *data, void *info)
{
    struct info_insert *ii = info;
    node_t *map = ii->info;
    node_t original = data->key;
    node_t copy = data->value;

    // copy attributed edges
    struct copy_attributed_edges cae = { map, copy };
    ae__access(U_QT, &original->attributed_edges, &cae, ae__predicate_1,
        copy_attributed_edges);

    // copy anything edge
    copy->anything_edge = nnm__get_value(map, original->anything_edge);
}


static
void allocate_and_map_copies(node_t *ref, const struct info_stack *info)
{
    node_t original = *ref;
    node_t *map = info->user;

    node_t copy = NULL;
    insert_state(&copy, original->is_accepting_state);

    void **odata = lexer__data(original);
    void **cdata = lexer__data(copy);
    *cdata = *odata;

    nnm__insert(map, (node_node_mapping){ original, copy}, nnm__cmp_predicate);
}


void lexer__copy(node_t *dst, node_t *src)
{
    node_t map = NULL;

    // allocate and map all copy nodes
    lexer__access(U_QT, src, &map, predicate_1, allocate_and_map_copies);

    // copy over edges replacing original references with copy references
    nnm__access(U_QT, &map, &map, nnm__predicate_1, copy_edges);

    // get copy of initial state
    *dst = nnm__get_value(&map, *src);
}
