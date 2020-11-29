#include <string.h>
#include "graph.h"
#include "path.h"
#include "queue.h"
#include "avl.h"
#include "text.h"
#include "rea.h"

// implementation to be lifted
// -----------------------------------------------------------------------------
static
void *get_user_info(void *info)
{
    struct infostack *is = info;
    return is->user;
}

static
void *get_impl_info(void *info)
{
    struct infostack *is = info;
    return is->impl;
}

static
void *path__node__data(void **ref)
{
    return node__data(*ref, path__edge_storage());
}

static
void *avl__node__data(void **ref)
{
    return node__data(*ref, avl__edge_storage());
}
// -----------------------------------------------------------------------------

struct edge_storage {
    void *attributed_edges;
    struct edge_storage *anything_edge;
    int accepting;
};

size_t rea__edge_storage(void)
{
    return sizeof(struct edge_storage);
}




static
int choose_char(char a, char b)
{
    return a < b ? 0 : 1;
}

static
int match_char(char a, char b)
{
    return a == b;
}


static
int choose_ptr(void *a, void *b)
{
    return a < b ? 0 : 1;
}

static
int match_ptr(void *a, void *b)
{
    return a == b;
}




// BEGIN attributed_edges management
// -----------------------------------------------------------------------------

struct attributed_edge {
    void *node;
    char byte;
};


struct insert_attributed_edge {
    size_t size;
    struct attributed_edge data;
};

static
int ae__insert_x(void **ref, void *info, int (*f)(char, char))
{
    struct attributed_edge *data = avl__node__data(ref);
    struct insert_attributed_edge *user = get_user_info(info);
    char a = user->data.byte;
    char b = data->byte;
    return f(a, b);
}

static
int ae__insert_choose(void **ref, void *info)
{
    return ae__insert_x(ref, info, choose_char);
}

static
void ae__insert(void **ref, void *node, char byte)
{
    struct insert_attributed_edge info = {
        sizeof(struct attributed_edge),
        { node, byte }
    };
    avl__insert(ref, &info, ae__insert_choose);
}


struct attributed_edge_key {
    char byte;
};

static
int ae__access_x(void **ref, void *info, int (*f)(char, char))
{
    struct attributed_edge *data = avl__node__data(ref);
    struct attributed_edge_key *user = get_user_info(info);
    char a = user->byte;
    char b = data->byte;
    return f(a, b);
}

static
int ae__access_choose(void **ref, void *info)
{
    return ae__access_x(ref, info, choose_char);
}

static
int ae__access_match(void **ref, void *info)
{
    return ae__access_x(ref, info, match_char);
}

static
void ae__access(int qt, void **ref, void *info,
    match_fn match, apply_fn apply)
{
    avl__access(qt, ref, info, ae__access_choose, match, apply);
}


struct attributed_edge_in {
    char byte; // key
    int in;
};

static
void ae__in_1(void **ref, void *info)
{
    if (*ref != NULL) {
        struct attributed_edge_in *user = get_user_info(info);
        user->in = 1;
    }
}

static
int ae__in(void **ref, char byte)
{
    struct attributed_edge_in info = { byte, 0 };
    ae__access(E_QT, ref, &info, ae__access_match, ae__in_1);
    return info.in;
}


struct attributed_edge_node_ref {
    char byte; // key
    void **ref;
};

static
void ae__get_node_ref(void **ref, void *info)
{
    struct attributed_edge *data = avl__node__data(ref);
    struct attributed_edge_node_ref *user = get_user_info(info);
    user->ref = &data->node; // node is first field
}

static
void **ae__reserve(void **ref, char byte)
{
    ae__insert(ref, NULL, byte);
    struct attributed_edge_node_ref info = { byte, NULL };
    ae__access(E_QT, ref, &info, ae__access_match, ae__get_node_ref);
    return info.ref;
}

static
struct attributed_edge *ae__get_edge(void **ref, char byte)
{
    struct attributed_edge_node_ref info = { byte, NULL };
    ae__access(E_QT, ref, &info, ae__access_match, ae__get_node_ref);
    return *info.ref;
}

static
struct attributed_edge *ae__get_edge2(void **ref, char byte)
{
    struct attributed_edge_node_ref info = { byte, NULL };
    ae__access(E_QT, ref, &info, ae__access_match, ae__get_node_ref);
    return (struct attributed_edge *)info.ref;
}

// END attributed_edges management
// -----------------------------------------------------------------------------




// BEGIN node_ref_set management
// -----------------------------------------------------------------------------

struct node_ref {
    void **ref;
};


struct insert_node_ref {
    size_t size;
    struct node_ref data;
};

static
int nrs__insert_x(void **ref, void *info, int (*f)(void *, void *))
{
    struct node_ref *data = avl__node__data(ref);
    struct insert_node_ref *user = get_user_info(info);
    void *a = *user->data.ref;
    void *b = *data->ref;
    return f(a, b);
}

static
int nrs__insert_choose(void **ref, void *info)
{
    return nrs__insert_x(ref, info, choose_ptr);
}

static
void nrs__insert(void **ref, void **node_ref)
{
    struct insert_node_ref info = {
        sizeof(struct node_ref),
        { node_ref }
    };
    avl__insert(ref, &info, nrs__insert_choose);
}


struct node_ref_key {
    void **ref;
};

static
int nrs__access_x(void **ref, void *info, int (*f)(void *, void *))
{
    struct node_ref *data = avl__node__data(ref);
    struct node_ref_key *user = get_user_info(info);
    void *a = *user->ref;
    void *b = *data->ref;
    return f(a, b);
}

static
int nrs__access_choose(void **ref, void *info)
{
    return nrs__access_x(ref, info, choose_ptr);
}

static
int nrs__access_match(void **ref, void *info)
{
    return nrs__access_x(ref, info, match_ptr);
}

static
void nrs__delete(void *ref, void **node_ref)
{
    struct node_ref_key info = { node_ref };
    avl__delete(ref, &info, nrs__access_choose, nrs__access_match);
}

static
void nrs__access(int qt, void **ref, void *info,
    match_fn match, apply_fn apply)
{
    avl__access(qt, ref, info, nrs__access_choose, match, apply);
}

struct node_ref_in {
    void **ref; // key
    int in;
};

static
void nrs__in_1(void **ref, void *info)
{
    if (*ref != NULL) {
        struct node_ref_in *user = get_user_info(info);
        user->in = 1;
    }
}

static
int nrs__in(void **ref, void **node_ref)
{
    struct node_ref_in info = { node_ref, 0 };
    nrs__access(E_QT, ref, &info, nrs__access_match, nrs__in_1);
    return info.in;
}


static
void nrs__get_node_ref(void **ref, void *info)
{
    if (*ref != NULL) {
        struct node_ref *data = avl__node__data(ref);
        struct node_ref *user = get_user_info(info);
        user->ref = data->ref;
    }
}

void **nrs__get_any(void **ref)
{
    struct node_ref info = { NULL };
    nrs__access(E_QT, ref, &info, match_1, nrs__get_node_ref);
    return info.ref;
}


static
void nrs__log_apply(void **ref, void *info)
{
    struct node_ref *data = avl__node__data(ref);
    fprintf(stderr, "%p %p\n", data->ref, *data->ref);
}

static
void nrs__log(void **ref)
{
    nrs__access(U_QT, ref, NULL, match_1, nrs__log_apply);
}

// END node_ref_set management
// -----------------------------------------------------------------------------




// BEGIN rea__access
// -----------------------------------------------------------------------------

struct info_impl_u {
    void *unvisited;
    void *visited;
};

static
void add_to_unvisited(struct info_impl_u *impl, void **node_ref)
{
    if (*node_ref != NULL
    && !nrs__in(&impl->visited, node_ref)
    && !nrs__in(&impl->unvisited, node_ref))
        nrs__insert(&impl->unvisited, node_ref);
}

static
void add_ae_to_unvisited(void **ref, void *info)
{
    if (*ref != NULL) {
        struct info_impl_u *user = get_user_info(info);
        struct attributed_edge *ae = node__data(*ref, avl__edge_storage());

        // add attributed edge children to unvisited
        add_to_unvisited(user, (void **)&ae->node);
    }
}

static
void next_u(void ***ref, void *info)
{
    struct info_impl_u *impl = get_impl_info(info);

    void **node_ref = *ref;
    struct edge_storage *node = *node_ref;

    // add current node to visited
    nrs__insert(&impl->visited, node_ref);

    // add attributed edges children to unvisited
    ae__access(U_QT, &node->attributed_edges, impl, match_1,
        add_ae_to_unvisited);

    // add anything edge child to unvisited
    add_to_unvisited(impl, (void **)&node->anything_edge);

    if (impl->unvisited == NULL)
        *ref = &impl->unvisited; // needs reference to NULL
    else
        *ref = nrs__get_any(&impl->unvisited);
    nrs__delete(&impl->unvisited, *ref);
}


static
void next_e(void ***ref, void *info)
{
    struct rea__buffer *user = get_user_info(info);
    struct info_impl_e *impl = get_impl_info(info);

    void **node_ref = *ref;
    struct edge_storage *node = *node_ref;


    if (node->accepting) {
        user->data_ref = node__data(node, sizeof(struct edge_storage));
        user->overread = 0;
    }

    struct attributed_edge *ae = NULL;
    if (node->attributed_edges != NULL)
        ae = ae__get_edge2(&node->attributed_edges, user->mem[user->cursor]);
    user->cursor++;
    user->overread++;
    if (ae == NULL)
        *ref = (void **)&node->anything_edge;
    else
        *ref = &ae->node;

    if (**ref == NULL) { // cannot continue
        user->cursor -= user->overread;
    }
}


void rea__access(int qt, void **ref, void *info,
    match_fn match, apply_fn apply)
{
    switch (qt) {
    case U_QT:
    {
        struct info_impl_u impl = { NULL, NULL };
        struct infostack is = { info, &impl };
        graph__uloop(ref, &is, match, next_u, apply);
    }
    break;
    case E_QT:
    {
        struct infostack is = { info, NULL };
        graph__eloop(ref, &is, match, next_e, apply);
    }
    break;
    }
}

// END rea__access
// -----------------------------------------------------------------------------




// BEGIN dump_dot
// -----------------------------------------------------------------------------

struct info_dump_dot_ext {
    struct info_dump_dot *idd;
    void *parent;
};

static
void dot_edge(void **ref, void *info)
{
    if (*ref != NULL) {
        struct infostack *is = info;
        struct info_dump_dot_ext *idde = is->user;
        FILE *fd = idde->idd->fd;
        void *parent = idde->parent;
        void *node = *ref;
        struct attributed_edge *e = node__data(node, avl__edge_storage());

        // escape non-printable characters
        char x[2];
        x[0] = e->byte;
        x[1] = 0;
        char y[10];
        text__escape(y, x);
        char z[10];
        text__escape(z, y);

        if (parent == e->node)
            fprintf(fd, "n%p:e->n%p:w[label=\"%s\"];", parent, e->node, z);
        else
            fprintf(fd, "n%p->n%p[label=\"%s\"];", parent, e->node, z);
    }
}

static
void apply_dump_dot(void **ref, void *info)
{
    struct edge_storage *node = *ref;
    struct infostack *is = info;
    struct info_dump_dot *i = is->user;
    FILE *fd = i->fd;
    void (*fpd)(FILE *, void **) = i->fpd;
    fprintf(fd, "n%p", node);
    if (node->accepting) {
        fprintf(fd, "[shape=doublecircle;label=\"");
        if (fpd != NULL)
            fpd(fd, ref);
        fprintf(fd, "\"];");
    } else {
        fprintf(fd, "[shape=circle;label=\"\"];");
    }
    struct info_dump_dot_ext idde = { i, node };
    avl__access(U_QT, &node->attributed_edges, &idde, NULL, match_1, dot_edge);
    // print anything edge
    if (node->anything_edge != NULL)
        fprintf(fd, "n%p->n%p[label=\".\";style=dashed];",
            node, node->anything_edge);
}

void rea__dump_dot(FILE *fd, void **ref, void (*fpd)(FILE *, void **))
{
    fprintf(fd, "digraph{rankdir=LR;");

    if (*ref != NULL) {
        fprintf(fd, "initial_state_arrow[shape=square;label=\"\"];");
        fprintf(fd, "initial_state_arrow->n%p;", *ref);
    }

    struct info_dump_dot info = { fd, fpd };
    rea__access(U_QT, ref, &info, match_1, apply_dump_dot);
    fprintf(fd, "}\n");
}

// END dump_dot
// -----------------------------------------------------------------------------




// BEGIN node_node_map
// -----------------------------------------------------------------------------

struct node_node_mapping {
    void *key;   // key node
    void *value; // value node
};


struct insert_node_node_mapping {
    size_t size;
    struct node_node_mapping data;
};

static
int nnm__insert_x(void **ref, void *info, int (*f)(void *, void *))
{
    struct node_node_mapping *data = avl__node__data(ref);
    struct insert_node_node_mapping *user = get_user_info(info);
    void *a = user->data.key;
    void *b = data->key;
    return f(a, b);
}

static
int nnm__insert_choose(void **ref, void *info)
{
    return nnm__insert_x(ref, info, choose_ptr);
}

static
void nnm__insert(void **ref, void *key, void *value)
{
    struct insert_node_node_mapping info = {
        sizeof(struct insert_node_node_mapping),
        { key, value }
    };
    avl__insert(ref, &info, nnm__insert_choose);
}


struct node_node_mapping_key {
    void *key;
};

static
int nnm__access_x(void **ref, void *info, int (*f)(void *, void *))
{
    struct node_node_mapping *data = avl__node__data(ref);
    struct node_node_mapping_key *user = get_user_info(info);
    void *a = user->key;
    void *b = data->key;
    return f(a, b);
}

static
int nnm__access_choose(void **ref, void *info)
{
    return nnm__access_x(ref, info, choose_ptr);
}

static
int nnm__access_match(void **ref, void *info)
{
    return nnm__access_x(ref, info, match_ptr);
}

static
void nnm__access(int qt, void **ref, void *info,
    match_fn match, apply_fn apply)
{
    avl__access(qt, ref, info, nnm__access_choose, match, apply);
}


static
void nnm__get_value_apply(void **ref, void *info)
{
    if (*ref != NULL) {
        struct node_node_mapping *data = avl__node__data(ref);
        struct node_node_mapping *user = get_user_info(info);
        user->value = data->value;
    }
}

static
void *nnm__get_value(void **ref, void *key)
{
    struct node_node_mapping info = { key, NULL };
    nnm__access(E_QT, ref, &info, nnm__access_match, nnm__get_value_apply);
    return info.value;
}

// END node_node_map
// -----------------------------------------------------------------------------




// BEGIN insert_state
// -----------------------------------------------------------------------------

static
void em_acc_x(void **ref, struct edge_storage *node, int accepting)
{
    node->attributed_edges = NULL;
    node->anything_edge = NULL;
    node->accepting = accepting;

    void **data_ptr = node__data(node, sizeof(struct edge_storage));
    *data_ptr = NULL;

    *ref = node;
}

static
void em_acc_0(void **ref, struct edge_storage *node, void *info)
{
    em_acc_x(ref, node, 0);
}

static
void em_acc_1(void **ref, struct edge_storage *node, void *info)
{
    em_acc_x(ref, node, 1);
}

struct insert_data_ptr { // temporary during initial development
    size_t size;
    void *data;
};

void insert_state(void **ref, int accepting)
{
    struct insert_data_ptr info = { sizeof(void *), NULL };
    struct infostack is = { &info, NULL };
    graph__insert(ref, &is, sizeof(struct edge_storage),
        accepting ? em_acc_1 : em_acc_0);
}

// END insert_state
// -----------------------------------------------------------------------------




// BEGIN BUILDERS
// =============================================================================

// BEGIN rea__literal
// -----------------------------------------------------------------------------

void rea__literal(void **dst, const char *lexeme)
{
    for (int i = 1; i < strlen(lexeme) - 1; i++) { // trim quotation marks
        insert_state(dst, 0);
        struct edge_storage *node = *dst;
        dst = ae__reserve(&node->attributed_edges, lexeme[i]);
    }
    insert_state(dst, 1);
}

// END rea__literal
// -----------------------------------------------------------------------------


// BEGIN rea__class
// -----------------------------------------------------------------------------

static
void rea__class__insert_character(void *is, void *as, char c)
{
    struct edge_storage *node = is;
    *ae__reserve(&node->attributed_edges, c) = as;
}

static
void rea__class__insert_range(void *is, void *as, char a, char b)
{
    if (a > b) {
        char temp = a;
        a = b;
        b = temp;
    }
    for (char c = a; c <= b; c++)
        rea__class__insert_character(is, as, c);
}

// a small parser is needed in this case for ranges
void rea__class(void **dst, const char *lexeme)
{
    insert_state(dst, 0);
    void *is = *dst; // initial state
    void *as = NULL; // accepting state
    insert_state(&as, 1);

    int s = 0;
    char buf;
    for (int i = 1; i < strlen(lexeme) - 1; i++) { // trim brackets
        char c = lexeme[i];
        switch (s) {
        case 0:
            buf = c;
            s = 1;
            break;
        case 1:
            switch (c) {
            case '-':
                s = 2;
                break;
            default:
                rea__class__insert_character(is, as, buf);
                buf = c;
                s = 1;
                break;
            }
            break;
        case 2:
            rea__class__insert_range(is, as, buf, c);
            s = 0;
            break;
        }
    }
    switch (s) {
    case 2:
        rea__class__insert_character(is, as, '-');
        // fallthrough
    case 1:
        rea__class__insert_character(is, as, buf);
        break;
    }
}

// END rea__class
// -----------------------------------------------------------------------------


// BEGIN rea__anything
// -----------------------------------------------------------------------------

void rea__anything(void **dst)
{
    insert_state(dst, 0);
    struct edge_storage *node = *dst;
    insert_state((void **)&node->anything_edge, 1);
}

// END rea__anything
// -----------------------------------------------------------------------------


// BEGIN rea__copy
// -----------------------------------------------------------------------------

struct node_node_map {
    void *map;
};


struct copy_attributed_edges {
    void *copy;
    void *map;
};

static
void copy_attributed_edges(void **ref, void *info)
{
    struct copy_attributed_edges *user = get_user_info(info);
    struct attributed_edge *ae = avl__node__data(ref);
    struct edge_storage *original = *ref;
    struct edge_storage *copy = user->copy;

    // insert edge in copy attributed edges changing the original reference
    // with the copy reference
    *ae__reserve(&copy->attributed_edges, ae->byte)
        = nnm__get_value(&user->map, ae->node);
}

static
void copy_edges(void **ref, void *info)
{
    struct node_node_map *user = get_user_info(info);
    struct node_node_mapping *data = avl__node__data(ref);
    struct edge_storage *original = data->key;
    struct edge_storage *copy = data->value;

    // copy attributed edges
    struct copy_attributed_edges cae = { copy, user->map };
    ae__access(U_QT, &original->attributed_edges, &cae, match_1,
        copy_attributed_edges);

    // copy anything edge
    copy->anything_edge = nnm__get_value(&user->map, original->anything_edge);
}


static
void allocate_and_map_copies(void **ref, void *info)
{
    struct edge_storage *original = *ref;
    struct edge_storage *copy = NULL;
    insert_state((void **)&copy, original->accepting);

    void **odata = node__data(original, sizeof(struct edge_storage));
    void **cdata = node__data(copy, sizeof(struct edge_storage));
    *cdata = *odata;

    struct node_node_map *user = get_user_info(info);
    nnm__insert(&user->map, original, copy);
}

void rea__copy(void **dst, void **src)
{
    struct node_node_map info = { NULL };

    // allocate and map all copy nodes
    rea__access(U_QT, src, &info, match_1, allocate_and_map_copies);

    // copy over edges replacing original references with copy references
    nnm__access(U_QT, &info.map, &info, match_1, copy_edges);

    // get copy initial state
    *dst = nnm__get_value(&info.map, *src);
}

// END rea__copy
// -----------------------------------------------------------------------------

// END BUILDERS
// =============================================================================




// BEGIN get_accepting_states
// -----------------------------------------------------------------------------

struct set_of_accepting_states {
    void *set;
};

struct node_edge_ref {
    void **ref;
};

struct insert_node_edge_ref {
    size_t size;
    struct node_edge_ref data;
};

static
void add_state(void **ref, void *info)
{
    struct set_of_accepting_states *user = get_user_info(info);
    struct insert_node_edge_ref iner = {
        sizeof(struct node_edge_ref),
        { ref }
    };
    path__insert(&user->set, &iner, match_1);
}

static
int match_accepting_states(void **ref, void *info)
{
    struct edge_storage *node = *ref;
    return node->accepting;
}

static
void *get_accepting_states(void **a)
{
    struct set_of_accepting_states info = { NULL };
    rea__access(U_QT, a, &info, match_accepting_states, add_state);
    return info.set;
}




struct refs_to_state {
    void **set;
    void *state;
};

static
void get_ae_edges_to_accepting(void **ref, void *info)
{
    struct attributed_edge *data = avl__node__data(ref);
    struct refs_to_state *user = get_user_info(info);

    if (data->node == user->state) {
        struct insert_node_edge_ref ner = {
            sizeof(struct node_edge_ref),
            &data->node
        };
        path__insert(user->set, &ner, match_1);
    }
}

static
void get_edges_to_accepting(void **ref, void *info)
{
    struct edge_storage *node = *ref;
    struct refs_to_state *user = get_user_info(info);

    // attributed edge
    ae__access(U_QT, &node->attributed_edges, user, match_1,
        get_ae_edges_to_accepting);

    // anything edge
    if (node->anything_edge == user->state) {
        struct insert_node_edge_ref ner = {
            sizeof(struct node_edge_ref),
            (void **)&node->anything_edge
        };
        path__insert(user->set, &ner, match_1);
    }
}

static
void get_accepting_states_refs_apply(void **ref, void *info)
{
    struct node_edge_ref *data = path__node__data(ref);
    struct refs_to_state *user = get_user_info(info);

    struct edge_storage *accepting_state = *data->ref;

    struct refs_to_state rts = { user->set, accepting_state };
    rea__access(U_QT, &user->state, &rts, match_1, get_edges_to_accepting);
}

static
void *get_accepting_states_refs(void **a)
{
    void *result_set = NULL;

    void *accepting_states = get_accepting_states(a);
    struct refs_to_state info = { &result_set, *a };
    path__access(U_QT, &accepting_states, &info, match_1,
        get_accepting_states_refs_apply);

    return result_set;
}

// END get_accepting_states
// -----------------------------------------------------------------------------




// BEGIN equivalence_class
// -----------------------------------------------------------------------------

struct equivalence_class {
    void *set;
};

struct ec_item {
    void *state;
};

struct insert_ec_item {
    size_t size;
    struct ec_item data;
};

static
void ec__add_state(void **ref, void *state)
{
    struct insert_ec_item info = {
        sizeof(struct ec_item),
        { state }
    };
    path__insert(ref, &info, match_0);
}

static
void ec__get_representative_apply(void **ref, void *info)
{
    struct ec_item *data = path__node__data(ref);
    struct ec_item *user = get_user_info(info);
    user->state = data->state;
}

static
void *ec__get_representative(void **ref)
{
    struct ec_item info = { NULL };
    path__access(E_QT, ref, &info, match_1, ec__get_representative_apply);
    return info.state;
}


struct ec_in {
    void *state; // key
    int in;
};

static
int ec__in_match(void **ref, void *info)
{
    struct ec_item *data = path__node__data(ref);
    struct ec_item *user = get_user_info(info);
    return data->state == user->state;
}

static
void ec__in_1(void **ref, void *info)
{
    if (*ref != NULL) {
        struct ec_in *user = get_user_info(info);
        user->in = 1;
    }
}

static
int ec__in(void **ref, void *state)
{
    struct ec_in info = { state, 0 };
    path__access(E_QT, ref, &info, ec__in_match, ec__in_1);
    return info.in;
}

// END equivalence_class
// -----------------------------------------------------------------------------




// BEGIN equivalence_class_queue
// -----------------------------------------------------------------------------

struct ecq_item {
    void *ec;
};

struct insert_ecq_item {
    size_t size;
    struct ecq_item data;
};

static
void ecq__insert(void **ref, void *ec)
{
    struct insert_ecq_item info = {
        sizeof(struct ecq_item),
        { ec }
    };
    queue__insert(ref, &info);
}

static
void ecq__delete(void **ref)
{
    queue__delete(ref);
}

static
void **ecq__access(void **ref)
{
    return queue__access(ref);
}

// END equivalence_class_queue
// -----------------------------------------------------------------------------




//
struct ec_repr_and_ecq {
    void *state;
    void **ecq;
};
static
void repl_uni_ae(void **ref, void *info)
{
    struct attributed_edge *data = avl__node__data(ref);
    struct ecq_item *user = get_user_info(info);

    struct edge_storage *representative = ec__get_representative(&user->ec);

    if (ec__in(&user->ec, data->node)) {
        data->node = representative;
    }
}

static
void repl_apply(void **ref, void *info)
{
    struct edge_storage *node = *ref;
    struct ecq_item *user = get_user_info(info);

    ae__access(U_QT, &node->attributed_edges, user, match_1, repl_uni_ae);

     struct edge_storage *representative = ec__get_representative(&user->ec);
    if (node->anything_edge != NULL && ec__in(&user->ec, node->anything_edge)) {
        node->anything_edge = representative;
    }
}
static
void replace_ec_states_with_repr2(void **ref, void *info)
{
    struct ecq_item *user = get_user_info(info);
    struct ec_item *data = path__node__data(ref);
    rea__access(U_QT, &data->state, user, match_1, repl_apply);
}

static
int is_not_repr(void **ref, void *info)
{
    struct ec_item *data = path__node__data(ref);
    struct ec_repr_and_ecq *user = get_user_info(info);
    return data->state != user->state;
}
// BEGIN optimize
// -----------------------------------------------------------------------------

struct cmp_ae {
    void *s1;
    int all_eq;
};

static
void cmp_ae_nall_eq(void **ref, void *info)
{
    if (*ref != NULL) {
        struct cmp_ae *user = get_user_info(info);
        user->all_eq = 0;
    }
}

static
int cmp_ae_match_neq(void **ref, void *info)
{
    struct cmp_ae *user = get_user_info(info);
    struct edge_storage *node = user->s1;
    struct attributed_edge *ae0 = avl__node__data(ref);
    struct attributed_edge *ae1 = NULL;
    if (ae__in(&node->attributed_edges, ae0->byte))
        ae1 = ae__get_edge2(&node->attributed_edges, ae0->byte);

    if (ae1 == NULL || ae0->node != ae1->node)
        return 1;
}

static
int states_equal(void *p0, void *p1)
{
    struct edge_storage *s0 = p0;
    struct edge_storage *s1 = p1;

    // comparisons ordered by cost

    if (s0->accepting != s1->accepting)
        return 0;

    if (s0->anything_edge != s1->anything_edge)
        return 0;

    // compare data pointer
    void **d0 = node__data(s0, sizeof(struct edge_storage));
    void **d1 = node__data(s1, sizeof(struct edge_storage));
    if (*d0 != *d1)
        return 0;

    // compare attributed edges

    // compare NULL case
    if (s0->attributed_edges == NULL || s1->attributed_edges == NULL)
        if (s0->attributed_edges != s1->attributed_edges)
            return 0;

    struct cmp_ae info = { s1, 1 };
    ae__access(E_QT, &s0->attributed_edges, &info,
        cmp_ae_match_neq, cmp_ae_nall_eq);
    return info.all_eq;
}


struct states_queue {
    void **ref;
};

struct sq_item {
    void *state;
};

struct insert_sq_item {
    size_t size;
    struct sq_item data;
};

static
void opt_append_states(void **ref, void *info)
{
    void *state = *ref;
    struct states_queue *user = get_user_info(info);

    struct insert_sq_item isi = {
        sizeof(struct sq_item),
        { state }
    };
    queue__insert(user->ref, &isi);
}

struct opt_bld_ec {
    void *repr;
    void **ec;
};

static
int opt_match_eq_state(void **ref, void *info)
{
    struct sq_item *data = path__node__data(ref);
    struct opt_bld_ec *user = get_user_info(info);
    return states_equal(user->repr, data->state);
}

static
void opt_bld_ec(void **ref, void *info)
{
    struct sq_item *data = path__node__data(ref);
    struct opt_bld_ec *user = get_user_info(info);
    if (*user->ec == NULL)
        ec__add_state(user->ec, user->repr);
    ec__add_state(user->ec, data->state);
}

static
int opt_match_in_ec(void **ref, void *info)
{
    struct sq_item *data = path__node__data(ref);
    struct ecq_item *user = get_user_info(info);
    return ec__in(&user->ec, data->state);
}

static
void optimize(void **ref)
{
    void *states_static = NULL;
    void *states = NULL;

    struct states_queue sq = { &states };
    rea__access(U_QT, ref, &sq, match_1, opt_append_states);
    sq.ref = &states_static;
    rea__access(U_QT, ref, &sq, match_1, opt_append_states);

    void *ec = NULL;
    int changes = 0;
    do {
        // get first states of the queue
        void *state = *(void **)queue__access(&states);
        queue__delete(&states);

        // build equivalence class with equal states
        struct opt_bld_ec obe = { state, &ec };
        path__access(U_QT, &states, &obe, opt_match_eq_state, opt_bld_ec);

        if (ec != NULL) {
            // in whole graph replace class nodes with representative
            struct ecq_item ei = { ec };
            path__access(U_QT, &states_static, &ei, is_not_repr, replace_ec_states_with_repr2);

            // update outer reference if necessary
            if (ec__in(&ec, *ref))
                *ref = ec__get_representative(&ec);

            // free non-representative nodes
            // TO DO

            // remove all nodes of class from queue
            struct ecq_item ei2 = { ec };
            path__delete(U_QT, &states, &ei2, opt_match_in_ec);

            // put representative to the queue
            struct insert_sq_item isi = {
                sizeof(struct sq_item),
                { ec__get_representative(&ec) }
            };
            if (states != NULL)
                queue__insert(&states, &isi);

            // empty equivalence class
            ec = NULL; // temporary
            changes = 1 && states != NULL;
        } else {
            changes = states != NULL;
        }
    } while (changes);
    // until no changes happened == no class built == no equivalent nodes
}

// END optimize
// -----------------------------------------------------------------------------




// BEGIN OPERATORS
// =============================================================================

// BEGIN rea__union
// -----------------------------------------------------------------------------



static
void rea__union_foreach_edge(void **ref, void *info)
{
    struct attributed_edge *data = avl__node__data(ref);
    struct ec_repr_and_ecq *user = get_user_info(info);

    struct edge_storage *representative = user->state;

    if (!ae__in(&representative->attributed_edges, data->byte)) {
        ae__insert(&representative->attributed_edges, data->node, data->byte);
        // delete edge
    } else {
        void *new_ec = NULL;
        ec__add_state(&new_ec, ae__get_edge(&representative->attributed_edges,
            data->byte));
        ec__add_state(&new_ec, data->node);
        ecq__insert(user->ecq, new_ec);
    }
}

static
void move_edges_to_repr(void **ref, void *info)
{
    struct ec_item *data = path__node__data(ref);
    struct ec_repr_and_ecq *user = get_user_info(info);

    if (data->state != NULL) { // APAÑO
        struct edge_storage *node = data->state;

        // attributed edges
        ae__access(U_QT, &node->attributed_edges, user, match_1,
            rea__union_foreach_edge);

        // anything edge
        struct edge_storage *repr = user->state;
        if (repr->anything_edge == NULL) {
            repr->anything_edge = node->anything_edge;
        } else {
            void *new_ec = NULL;
            ec__add_state(&new_ec, repr->anything_edge);
            ec__add_state(&new_ec, node->anything_edge);
            ecq__insert(user->ecq, new_ec);
        }

        // union of accepted
        repr->accepting = repr->accepting || node->accepting;
    }
}

static
void replace_ec_states_with_repr(void **dst, void **ec)
{
    void *repr = ec__get_representative(ec);

    // treat outermost reference
    if (ec__in(ec, *dst)) {
        *dst = repr;
    }

    struct ecq_item info = { *ec };
    rea__access(U_QT, &repr, &info, match_1, repl_apply);
}



void rea__union(void **dst, void **src)
{
    void *init_ec = NULL;
    ec__add_state(&init_ec, *dst);
    ec__add_state(&init_ec, *src);

    void *ecq = NULL;
    ecq__insert(&ecq, init_ec);

    while (ecq != NULL) {
        void **ec = ecq__access(&ecq);

        // copy non-representative node's edges to representative
        struct ec_repr_and_ecq info = { ec__get_representative(ec), &ecq };
        path__access(U_QT, ec, &info, is_not_repr, move_edges_to_repr);

        // starting from each state in the equivalence class replace
        // non-representatives' adresses with representative's address
        replace_ec_states_with_repr(dst, ec);
        struct ecq_item ei = { *ec };
        path__access(U_QT, ec, &ei, is_not_repr, replace_ec_states_with_repr2);

        ecq__delete(&ecq);
    }
    optimize(dst);
}

// END rea__union
// -----------------------------------------------------------------------------


struct pass_src {
    void **src;
};

// BEGIN rea__concatenation
// -----------------------------------------------------------------------------

static
void aux_cat_union(void **ref, void *info)
{
    struct node_edge_ref *data = path__node__data(ref);
    struct pass_src *user = get_user_info(info);

    // leftmost is representative
    rea__union(data->ref, user->src);

    struct edge_storage *node = *data->ref;
    struct edge_storage *nsrc = *user->src;
    node->accepting = nsrc->accepting;
}

void rea__concatenation(void **dst, void **src)
{
    // union of dst accepting states and src initial state
    void *as = get_accepting_states_refs(dst);
    struct pass_src info = { src };
    path__access(U_QT, &as, &info, match_1, aux_cat_union);
    optimize(dst);
}

// END rea__concatenation
// -----------------------------------------------------------------------------


// BEGIN rea__kleene_star
// -----------------------------------------------------------------------------

static
void aux_ks_union(void **ref, void *info)
{
    struct node_edge_ref *data = path__node__data(ref);
    struct pass_src *user = get_user_info(info);

    rea__union(user->src, data->ref);

    struct edge_storage *node = *data->ref;
    node->accepting = 1;
}

void rea__kleene_star(void **dst)
{
    // union of dst accepting states and dst initial state
    void *as = get_accepting_states_refs(dst);
    struct pass_src info = { dst };
    path__access(U_QT, &as, &info, match_1, aux_ks_union);
    optimize(dst);
}

// END rea__kleene_star
// -----------------------------------------------------------------------------


// BEGIN rea__kleene_plus
// -----------------------------------------------------------------------------

void rea__kleene_plus(void **dst)
{
    void *copy = NULL;
    rea__copy(&copy, dst);
    rea__kleene_star(&copy);
    rea__concatenation(dst, &copy);
    optimize(dst);
}

// END rea__kleene_plus
// -----------------------------------------------------------------------------

// END OPERATORS
// =============================================================================




// BEGIN rea__set_accepting_states_data
// -----------------------------------------------------------------------------

struct pass_data {
    void *ptr;
};

static
void set_data_ptr(void **ref, void *info)
{
    struct node_edge_ref *data = path__node__data(ref);
    struct edge_storage *node = *data->ref;
    struct pass_data *user = get_user_info(info);
    void **dptr = node__data(node, sizeof(struct edge_storage));
    *dptr = user->ptr;
}

void rea__set_accepting_states_data(void **ref, void *data)
{
    void *as = get_accepting_states(ref);

    struct pass_data info = { data };
    path__access(U_QT, &as, &info, match_1, set_data_ptr);
}

// END rea__set_accepting_states_data
// -----------------------------------------------------------------------------
