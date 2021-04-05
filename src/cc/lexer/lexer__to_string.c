#include <stdlib.h>
#include <string.h>
#include "libsapling/cc/lexer.h"
#include "libsapling/dm/avl.h"
#include "cc/lexer/lexer_edge_storage.h"
#include "cc/lexer/lexer_attributed_edges.h"

typedef struct {
    node_t key;
    int value;
} node_int_mapping;

static
int nim_cmp(const void *a, const void *b)
{
    return ((node_int_mapping *)a)->key <= ((node_int_mapping *)b)->key;
}

static
int nim_equ(const void *a, const void *b)
{
    return ((node_int_mapping *)a)->key == ((node_int_mapping *)b)->key;
}

static
void nim_fpf(FILE *stream, const void *data)
{
    node_int_mapping *m = (node_int_mapping *)data;
    fprintf(stream, "(%p, %d)", m->key, m->value);
}

IMPLEMENT_TYPED_AVL(nim, node_int_mapping, nim_cmp, nim_equ, nim_fpf)

static
void nim__get_value_apply(node_int_mapping *data, void *info)
{
    struct info_insert *ii = info;
    node_int_mapping *user = ii->info;
    user->value = data->value;
}

static
int nim__get_value(node_t *ref, node_t key)
{
    node_int_mapping m = { .key = key };
    nim__access(E_QT, ref, &m, nim__equ_predicate, nim__get_value_apply);
    return m.value;
}


struct enumerate {
    node_t map;
    int cnt;
};

static
void assign_number(node_t *ref, const struct info_stack *info)
{
    struct enumerate *user = info->user;

    nim__insert(&user->map, (node_int_mapping){ *ref, user->cnt++ },
        nim__cmp_predicate);
}

node_t lexer__enumerate_states(node_t *ref)
{
    struct enumerate info = { NULL, 0 };
    lexer__access(U_QT, ref, &info, predicate_1, assign_number);
    return info.map;
}

struct pretty_print_ae {
    node_t *map;
    char *buf;
};

static
void pretty_print_ae(attributed_edge *data, void *info)
{
    struct info_insert *ii = info;
    struct pretty_print_ae *user = ii->info;

    sprintf(user->buf + strlen(user->buf), "%c->%2d ", data->byte, nim__get_value(user->map, data->node));
}

const char *lexer__to_string(node_t *ref, node_t *map)
{
    node_t node = *ref;
    char *str = malloc(sizeof(char) * 4096);
    *str = 0;
    sprintf(str + strlen(str), "s%2d: ", nim__get_value(map, node));
    sprintf(str + strlen(str), node->is_accepting_state ? "acc " : "    ");
    struct pretty_print_ae info = { map, str };
    ae__access(U_QT, &node->attributed_edges, &info, ae__predicate_1, pretty_print_ae);
    sprintf(str + strlen(str), "\n");
    return str;
}

