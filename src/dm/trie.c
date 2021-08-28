#include <stdlib.h>
#include <string.h>
#include "libsapling/dm/path.h"
#include "libsapling/dm/stack.h"
#include "libsapling/dm/queue.h"
#include "libsapling/cc/lexer.h"
#include "cc/lexer/lexer_edge_storage.h"
#include "cc/lexer/lexer_attributed_edges.h"

void *trie__data(const node_t node)
{
    return lexer__data(node);
}


IMPLEMENT_TYPED_STACK(cs, char, char__print)
IMPLEMENT_TYPED_PATH(cp, char, char__print, dummy_cmp)


typedef attributed_edge *ae_ptr;

static
void ae_ptr__fpf(FILE *stream, const void *data)
{
    attributed_edge **ae = (attributed_edge **)data;

    attributed_edge__fpf(stream, *ae);
}

IMPLEMENT_TYPED_QUEUE(trail1, ae_ptr, ae_ptr__fpf)

IMPLEMENT_TYPED_STACK(ps, pnode_t, NULL)


static
void print_trail0(FILE *stream, const void *data)
{
    const node_t **q = data;

    trail1__print_data(stream, *q);
}

typedef struct { node_t *q; } trail_t;
IMPLEMENT_TYPED_STACK(trail0, trail_t, NULL)
IMPLEMENT_TYPED_PATH(trail0p, trail_t, print_trail0, dummy_cmp)



struct info_impl_u {
    node_t s; // need a stack of queues to reconstruct key
};


static
void insert_ae_in_queue(attributed_edge *data, void *info)
{
    CAST_USER_INFO(node_t *, q, info);

    trail1__insert(q, data);
}

static
node_t *next_key(node_t *ref, node_t *stack)
{
    // put all attributed edges on the queue (trail1)
    {
        node_t *q = malloc(sizeof(node_t));
        *q = NULL;
        node_t node = *ref;
        ae__access(U_QT, &node->attributed_edges, q, ae__predicate_1,
            insert_ae_in_queue);
        // put queue on the stack (trail0)
        trail0__insert(stack, (trail_t){ q });
    }
    while (*stack != NULL) {
        // access top of stack
        trail_t t = trail0__access(stack);


        while (*t.q == NULL) {
            trail0__delete(stack);
            if (*stack == NULL)
                return NULL;
            t = trail0__access(stack);
            trail1__delete(t.q);
    
        }
        // access queue
        attributed_edge *ae = trail1__access(t.q);
        // follow node
        node_t node = ae->node;
        if (node->is_accepting_state) {
            return &ae->node;
        }
        // put all attributed edges on the queue (trail1)
        node_t *q = malloc(sizeof(node_t));
        *q = NULL;
        ae__access(U_QT, &node->attributed_edges, q, ae__predicate_1,
            insert_ae_in_queue);
        // put queue on the stack (trail0)
        trail0__insert(stack, (trail_t){ q });
    }
    return NULL;
}

static
node_t *first_key(node_t *ref, node_t *stack)
{
    if (*ref == NULL) {
        return ref;
    } else {
        // prepare root stack
        {
            node_t node = *ref;
            // put all attributed edges on the queue (trail1)
            node_t *q = malloc(sizeof(node_t)); *q = NULL;
            ae__access(U_QT, &node->attributed_edges, q, ae__predicate_1,
                insert_ae_in_queue);
            // put queue on the stack (trail0)
            trail0__insert(stack, (trail_t){ q });
        }
        while (1) {
            // access top of stack
            trail_t t = trail0__access(stack);
            // access queue
            attributed_edge *ae = trail1__access(t.q);
            // follow node
            node_t node = ae->node;
            if (node->is_accepting_state) {
                return &ae->node;
            }
            // put all attributed edges on the queue (trail1)
            node_t *q = malloc(sizeof(node_t));
            *q = NULL;
            ae__access(U_QT, &node->attributed_edges, q, ae__predicate_1,
                insert_ae_in_queue);
            // put queue on the stack (trail0)
            trail0__insert(stack, (trail_t){ q });
        }
    }
    return NULL;
}


static
int next_u(node_t **ref, const struct info_stack *info)
{
    struct info_impl_u *impl = info->impl;

    *ref = next_key(*ref, &impl->s);
    return *ref != NULL;
}


struct info_impl_e {
    node_t s; // need stack of path taken for deletion
    const char *key;
};

static
node_t *search_key(node_t *ref, const struct info_stack *info)
{
    struct info_impl_e *impl = info->impl;

    pnode_t pnode = ref;
    int i = 0;
    while (pnode != NULL && *pnode != NULL && i < strlen(impl->key)) {
        node_t node = *pnode;
        ps__insert(&impl->s, pnode);
        attributed_edge *ae = ae__get_edge(&node->attributed_edges,
            impl->key[i]);
        pnode = &ae->node;
        i++;
    }

    return pnode;
}


void trie__access(enum qt qt, node_t *ref, const char *key, void *info,
    predicate_t predicate, apply_t apply)
{
    if (qt == U_QT) {
        struct info_impl_u impl = { NULL };
        struct info_stack is = { info, &impl };
        ref = first_key(ref, &impl.s);
        graph__access(qt, ref, &is, predicate, apply, next_u);
    } else { // qt == E_QT
        struct info_impl_e impl = { NULL, key };
        struct info_stack is = { info, &impl };
        ref = search_key(ref, &is);
        if (ref == NULL || *ref == NULL || !(*ref)->is_accepting_state)
            return; // key not found
        graph__access(qt, ref, &is, predicate, apply, NULL);
    }
}


static
void apply_union_of_data(node_t *ref, const struct info_stack *info)
{
    *(void **)trie__data(*ref) = info->user;
}

void trie__insert(node_t *ref, const char *key, void *info)
{
    char *buf = malloc(strlen(key) + 3);
    char *bufptr = buf;
    *bufptr++ = '\"';
    for (int i = 0; i < strlen(key); i++)
        *bufptr++ = key[i];
    *bufptr++ = '\"';
    *bufptr++ = '\0';

    node_t a = NULL;
    lexer__literal(&a, buf);
    lexer__set_accepting_states_data(&a, info);

    free(buf);

    if (*ref == NULL)
        *ref = a;
    else {
        lexer__union(ref, &a);
        // union of data pointer
        trie__access(E_QT, ref, key, info, predicate_1, apply_union_of_data);
    }
}


static
void del_em_2(node_t *ref, const node_t node, UNUSED void *impl_ptr)
{
    *ref = NULL;
}

static
void del_em(node_t *ref, const node_t node, void *impl_ptr)
{
    struct info_impl_e *impl = impl_ptr;

    int i = strlen(impl->key) - 1;
    while (impl->s != NULL) {
        char c = impl->key[i--];
        pnode_t pnode = ps__access(&impl->s);
        node_t node = *pnode;
        ae__delete(&node->attributed_edges, (attributed_edge){ .byte = c },
            ae__equ_predicate);

        // if no edges left delete node
        if (node->attributed_edges == NULL) {
            struct info_stack is = { NULL, impl_ptr };
            graph__delete(pnode, &is, del_em_2);
        } else {
            break; // is a prefix
        }

        ps__delete(&impl->s);
    }
}

static
void delete(node_t *ref, const struct info_stack *info)
{
    node_t node = *ref;

    // delete only data if has successors
    if (node->attributed_edges != NULL) {
        void **data = trie__data(node);
        *data = NULL;
        node->is_accepting_state = 0;
    } else {
        graph__delete(ref, info, del_em);
    }
}

void trie__delete(node_t *ref, const char *key)
{
    trie__access(E_QT, ref, key, NULL, predicate_1, delete);
}

static
void all_access_adapter(node_t *ref, void *info, apply_t apply)
{
    trie__access(U_QT, ref, NULL, info, predicate_1, apply);
}

static
void push_key_chars(trail_t *data, void *info)
{
    CAST_USER_INFO(node_t *, krb, info);

    if (data != NULL && data->q != NULL && *data->q != NULL)
        cs__insert(krb, trail1__access(data->q)->byte);
}

static
void print_data_aux0(FILE *stream, const node_t node, fpfdata_t fpfdata,
    void *impl)
{
    struct info_impl_u *impl_u = impl;

    // reconstruct and print key
    // run through the whole stack and get the first characters in the queue
    /*fprintf(stderr, "DEBUG\n");
    trail0p__print_data(stderr, &impl_u->s);
    fprintf(stderr, "\n\n");*/

    fprintf(stream, "(\"");
    node_t krb = NULL;
    trail0p__access(U_QT, &impl_u->s, &krb, trail0p__predicate_1,
        push_key_chars);
    char *key = malloc((cp__length(&krb) + 1) * sizeof(char));
    key[cp__length(&krb)] = 0;

    int static_length = cp__length(&krb);
    for (int i = 0; i < static_length; i++) {
        key[i] = cs__delete(&krb);
    }

    fprintf(stream, key);
    fprintf(stream, "\", ");
    void *data = *(void **)trie__data(node);
    if (fpfdata != NULL && data != NULL)
        fpfdata(stream, data);
    fprintf(stream, ")");
}

void trie__print_data(FILE *stream, node_t *ref, fpfdata_t fpfdata)
{
    graph__print_data(stream, ref, all_access_adapter, print_data_aux0,
        fpfdata);
}

int trie__length(const node_t *ref)
{
    return graph__length(ref, all_access_adapter);
}
