#include "libsapling/dm/avl.h"
#include "libsapling/cc/text.h"
#include "libsapling/cc/lexer.h"
#include "cc/lexer/lexer_edge_storage.h"
#include "cc/lexer/lexer_attributed_edges.h"


void *lexer__data(const node_t node)
{
    return graph__data(node, sizeof(struct edge_storage));
}


/*
 * Gee. Don't forget that this implementation discriminates by node address
 * and not by node reference address. That's why double dereferencing. One for
 * the typed generic pointer and another one to reach the node's address.
 * Otherwise you'll end up with Graphviz plotting the same edges times there
 * are references to the node the edges sprout from.
 */

static
int pnode_t__cmp(const void *a, const void *b)
{
    return **(node_t **)a <= **(node_t **)b;
}

static
int pnode_t__equ(const void *a, const void *b)
{
    return **(node_t **)a == **(node_t **)b;
}

static
void pnode_t__fpf(FILE *stream, const void *data)
{
    fprintf(stream, "%p->%p", *(node_t **)data, **(node_t **)data);
}

IMPLEMENT_TYPED_AVL(nrs, node_t *, pnode_t__cmp, pnode_t__equ, pnode_t__fpf)

static
void nrs__get_node_ref(node_t **data, void *info)
{
    CAST_USER_INFO(node_t **, user, info);

    *user = *data;
}

static
node_t *nrs__get_any(node_t *ref)
{
    node_t *elem = NULL;
    nrs__access(E_QT, ref, &elem, nrs__predicate_1, nrs__get_node_ref);
    return elem;
}


struct info_impl_u {
    node_t unvisited;
    node_t visited;
};

static
void add_to_unvisited(struct info_impl_u *impl, node_t *ref)
{
    if (*ref != NULL &&
        !nrs__in(&impl->visited, ref) &&
        !nrs__in(&impl->unvisited, ref)) {
        nrs__insert(&impl->unvisited, ref, nrs__cmp_predicate);
    }
}

static
void add_ae_to_unvisited(attributed_edge *data, void *info)
{
    CAST_USER_INFO(struct info_impl_u *, user, info);

    // add attributed edge target node to unvisited
    add_to_unvisited(user, &data->node);
}

static
int next_u(node_t **ref, const struct info_stack *info)
{
    node_t node = **ref;
    struct info_impl_u *impl = info->impl;

    node_t *pnode = *ref;
    // add current node to visited
    nrs__insert(&impl->visited, pnode, nrs__cmp_predicate);

    // add attributed edges children to unvisited
    ae__access(U_QT, &node->attributed_edges, impl, ae__predicate_1,
        add_ae_to_unvisited);

    // add anything edge to unvisited
    add_to_unvisited(impl, &node->anything_edge);

    // select next node to visit
    *ref = nrs__get_any(&impl->unvisited);
    nrs__delete(&impl->unvisited, *ref, nrs__equ_predicate);
    return *ref != NULL;
}




static
int next_e(node_t **ref, const struct info_stack *info)
{
    node_t node = **ref;
    struct lexer_state *user = info->user;

    if (node->is_accepting_state) {
        user->ref_to_data = lexer__data(node);
        user->overread = 0;
    }

    if (user->buf[user->cursor] == 0) {
        user->lexeme[user->lexeme_cursor] = 0;
        user->lexeme_cursor = 0;
        user->end = 1;
        return 0;
    }

    attributed_edge *ae = NULL;
    if (node->attributed_edges != NULL)
        ae = ae__get_edge(&node->attributed_edges, user->buf[user->cursor]);
    user->lexeme[user->lexeme_cursor++] = user->buf[user->cursor];
    user->cursor++;
    user->overread++;
    if (ae == NULL)
        *ref = &node->anything_edge;
    else
        *ref = &ae->node;

    // cannot continue, return to last accepting state
    if (**ref == NULL) {
        user->cursor -= user->overread;
        user->lexeme_cursor -= user->overread;
        user->lexeme[user->lexeme_cursor] = 0;
        user->lexeme_cursor = 0;
    }

    return 1;
}


void lexer__access(enum qt qt, node_t *ref, void *info, predicate_t predicate,
    apply_t apply)
{
    struct info_impl_u impl = { NULL, NULL };
    struct info_stack is = { info, &impl };
    next_t next = qt == U_QT ? next_u : next_e;
    graph__access(qt, ref, &is, predicate, apply, next);
}


static
void all_access_adapter(node_t *ref, void *info, apply_t apply)
{
    lexer__access(U_QT, ref, info, predicate_1, apply);
}

struct info_dump_dot_ext {
    FILE *stream;
    node_t parent;
};

static
void dot_edge(attributed_edge *data, void *info)
{
    CAST_USER_INFO(struct info_dump_dot_ext *, idde, info);

    FILE *stream = idde->stream;
    node_t parent = idde->parent;

    // escape non-printable characters
    char x[2] = { data->byte, 0 };
    char y[10];
    text__escape(y, x);
    char z[10];
    text__escape(z, y);

    if (parent == data->node)
        fprintf(stream, "n%p:e->n%p:w[label=\"%s\"];", parent, data->node, z);
    else
        fprintf(stream, "n%p->n%p[label=\"%s\"];", parent, data->node, z);
}

static
void dump_dot_aux0(FILE *stream, const node_t node, fpfdata_t fpfdata,
    void *info)
{
    fprintf(stream, "n%p", node);
    if (node->is_accepting_state) {
        fprintf(stream, "[shape=doublecircle;label=\"");
        void *data = *(void **)lexer__data(node);
        if (fpfdata != NULL && data != NULL)
            fpfdata(stream, data);
        fprintf(stream, "\"];");
    } else {
        fprintf(stream, "[shape=circle;label=\"\"];");
    }

    // attributed edges
    struct info_dump_dot_ext idde = { stream, node };
    ae__access(U_QT, &node->attributed_edges, &idde, ae__predicate_1,
        dot_edge);

    // anything edge
    if (node->anything_edge != NULL) {
        fprintf(stream, "n%p->n%p[label=\".\";style=dashed];", node,
            node->anything_edge);
    }
}

void lexer__dump_dot(FILE *stream, node_t *ref, fpfdata_t fpfdata)
{
    graph__dump_dot(stream, ref, all_access_adapter, dump_dot_aux0, fpfdata,
        "rankdir=LR;");
}

static
void print_aux0(FILE *stream, const node_t node, fpfdata_t fpfdata,
    void *impl)
{
    fprintf(stream, "(");
    fprintf(stream, "%p", node);
    fprintf(stream, ", ");
    ae__print(stream, &node->attributed_edges);
    fprintf(stream, ", ");
    fprintf(stream, "%p", node->anything_edge);
    fprintf(stream, ", ");
    fprintf(stream, "%d", node->is_accepting_state);
    fprintf(stream, ", ");
    void *data = *(void **)lexer__data(node);
    if (fpfdata != NULL && data != NULL)
        fpfdata(stream, data);
    else
        if (fpfdata != NULL)
            fprintf(stream, "%p", data);
    fprintf(stream, ")");
}

void lexer__print(FILE *stream, node_t *ref, fpfdata_t fpfdata)
{
    graph__print(stream, ref, all_access_adapter, print_aux0, fpfdata);
}


void lexer__println(FILE *stream, node_t *ref, fpfdata_t fpfdata)
{
    graph__println(stream, ref, all_access_adapter, print_aux0, fpfdata);
}


void lexer__init(struct lexer_state *ref, const char *buf)
{
    ref->buf = buf;
    ref->cursor = 0;
    ref->overread = 0;
    ref->ref_to_data = NULL;
    ref->lexeme_cursor = 0;
    ref->end = 0;
}

static
void do_nothing(node_t *ref, const struct info_stack *info)
{
}

void **lexer__next_terminal(node_t *ref, struct lexer_state *sta)
{
    lexer__access(E_QT, ref, sta, predicate_0, do_nothing);
    return sta->ref_to_data;
}

int lexer__length(const node_t *ref)
{
    return graph__length(ref, all_access_adapter);
}
