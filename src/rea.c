#include <string.h>
#include "graph.h"
#include "path.h"
#include "avl.h"

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
    user->ref = &data->node;
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

struct info_impl {
    void *unvisited;
    void *visited;
};

static
void add_to_unvisited(struct info_impl *impl, void **node_ref)
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
        struct info_impl *user = get_user_info(info);
        struct attributed_edge *ae = node__data(*ref, avl__edge_storage());

        // add attributed edge children to unvisited
        add_to_unvisited(user, (void **)&ae->node);
    }
}

static
void next_u(void ***ref, void *info)
{
    struct info_impl *impl = get_impl_info(info);

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


void rea__access(int qt, void **ref, void *info,
    match_fn match, apply_fn apply)
{
    struct info_impl impl = { NULL, NULL };
    struct infostack is = { info, &impl };
    switch (qt) {
    case U_QT:
        graph__uloop(ref, &is, match, next_u, apply);
        break;
    case E_QT:
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
        if (parent == e->node)
            fprintf(fd, "n%p:e->n%p:w[label=\"%c\"];", parent, e->node,
                e->byte);
        else
            fprintf(fd, "n%p->n%p[label=\"%c\"];", parent, e->node, e->byte);
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

struct insert_no_data { // temporary during initial development
    size_t size;
};

void insert_state(void **ref, int accepting)
{
    struct insert_no_data info = { 0 };
    struct infostack is = { &info, NULL };
    graph__insert(ref, &is, sizeof(struct edge_storage),
        accepting ? em_acc_1 : em_acc_0);
}

// END insert_state
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

    struct node_node_map *user = get_user_info(info);
    nnm__insert(&user->map, original, copy);
}

void *rea__copy(void *a)
{
    struct node_node_map info = { NULL };

    // allocate and map all copy nodes
    rea__access(U_QT, &a, &info, match_1, allocate_and_map_copies);

    // copy over edges replacing original references with copy references
    nnm__access(U_QT, &info.map, &info, match_1, copy_edges);

    // get copy initial state
    return nnm__get_value(&info.map, a);
}

// END rea__copy
// -----------------------------------------------------------------------------




// BEGIN BUILDERS
// =============================================================================

// BEGIN rea__literal
// -----------------------------------------------------------------------------

void *rea__literal(const char *lexeme)
{
    void *initial_state = NULL;
    void **ref = &initial_state;
    for (int i = 1; i < strlen(lexeme) - 1; i++) { // trim quotation marks
        insert_state(ref, 0);
        struct edge_storage *node = *ref;
        ref = ae__reserve(&node->attributed_edges, lexeme[i]);
    }
    insert_state(ref, 1);
    return initial_state;
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
void *rea__class(const char *lexeme)
{
    void *is = NULL;
    void *as = NULL;
    insert_state(&is, 0);
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

    return is;
}

// END rea__class
// -----------------------------------------------------------------------------


// BEGIN rea__anything
// -----------------------------------------------------------------------------

void *rea__anything(void)
{
    void *initial_state = NULL;
    void **ref = &initial_state;
    insert_state(ref, 0);
    struct edge_storage *node = *ref;
    insert_state((void **)&node->anything_edge, 1);
    return initial_state;
}

// END rea__anything
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
void *get_accepting_states_refs(void *a)
{
    void *result_set = NULL;

    void *accepting_states = get_accepting_states(&a);
    struct refs_to_state info = { &result_set, a };
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
    struct ec_in *user = get_user_info(info);
    user->in = 1;
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




// BEGIN OPERATORS
// =============================================================================

// BEGIN rea__union
// -----------------------------------------------------------------------------
// pseudocode:
// create equivalence class with both initial states
// choose first state as representantive
// for each state in the equivalence class that is not the representative
//   for each edge
//     if edge is not in representatives' edges
//        add edge to representative alongside its target (subautomaton)
//        delete edge
//     else
//       create equivalence class with
//         representative's edge target and this edge's target, choose first as
//         representative
//   delete state

struct ec_representative {
    void *state;
};

void *rea__union(void *a0, void *a1);

static
void rea__union_foreach_edge(void **ref, void *info)
{
    struct attributed_edge *data = avl__node__data(ref);
    struct ec_representative *user = get_user_info(info);

    struct edge_storage *representative = user->state;

    if (!ae__in(&representative->attributed_edges, data->byte)) {
        ae__insert(&representative->attributed_edges, /**/data->node, data->byte);
        // delete edge
    } else {
        rea__union(ae__get_edge(&representative->attributed_edges, data->byte),
            data->node);
    }
}

static
void rea__union_foreach_state(void **ref, void *info)
{
    struct ec_item *data = path__node__data(ref);
    struct ec_representative *user = get_user_info(info);

    if (data->state != NULL) { // APAÑO
        struct edge_storage *node = data->state;

        // attributed edges
        if (node->attributed_edges != NULL)
        ae__access(U_QT, &node->attributed_edges, user, match_1,
            rea__union_foreach_edge);

        // anything edge
        struct edge_storage *repr = user->state;
        if (repr->anything_edge == NULL) {
            repr->anything_edge = node->anything_edge;
        } else {
            rea__union(repr->anything_edge, node->anything_edge);
        }
    }
}

static
int is_not_representative(void **ref, void *info)
{
    struct ec_item *data = path__node__data(ref);
    struct ec_representative *user = get_user_info(info);
    return data->state != user->state;
}

void *rea__union(void *a0, void *a1)
{
    void *ec = NULL;
    ec__add_state(&ec, a0);
    ec__add_state(&ec, a1);

    struct ec_representative info = { ec__get_representative(&ec) };
    path__access(U_QT, &ec, &info, is_not_representative,
        rea__union_foreach_state);
    return a0;
}

// END rea__union
// -----------------------------------------------------------------------------


// BEGIN rea__concatenation
// -----------------------------------------------------------------------------

struct initial_state {
    void *state;
    void *global; // global initial state
};

static
void rea__concatenation_apply(void **ref, void *info)
{
    struct node_edge_ref *data = path__node__data(ref);
    struct initial_state *user = get_user_info(info);
    rea__union(user->state, *data->ref);
    int global_state_changes = user->global == *data->ref;
    *data->ref = user->state;
    if (global_state_changes)
        user->global = *data->ref;
}

struct apan0 {
    void **ec;
};

static
int apano0(void **ref, void *info)
{
    struct node_ref *data = path__node__data(ref);
    struct apan0 *user = get_user_info(info);
    return ec__in(user->ec, *data->ref);
}

void *rea__concatenation(void *a0, void *a1)
{
    // NOTES
    // union of a0's accepting states with a1's initial state
    // save union in a1's initial state and free a0's accepting states
    // unless a0's initial state is one of a1's accepting states? kleene star?
    // point the edges previously pointing to a1's accepting states to a0's
    // initial state

    void *accepting_states = get_accepting_states_refs(a0);
    void *initial_state = a1;

    struct initial_state info = { initial_state, a0 };
    path__access(U_QT, &accepting_states, &info, match_1,
        rea__concatenation_apply);

    return info.global;
}

// END rea__concatenation
// -----------------------------------------------------------------------------


// BEGIN rea__kleene_star
// -----------------------------------------------------------------------------

void *rea__kleene_star(void *a)
{
    // union of a's initial state with a's accepting states
    void *accepting_states = get_accepting_states_refs(a);
    void *initial_state = a;

    struct initial_state info = { initial_state };
    path__access(U_QT, &accepting_states, &info, match_1,
        rea__concatenation_apply);

    struct edge_storage *node = a;
    node->accepting = 1;

    return a;
}

// END rea__kleene_star
// -----------------------------------------------------------------------------


// BEGIN rea__kleene_plus
// -----------------------------------------------------------------------------

void *rea__kleene_plus(void *a)
{
    return rea__concatenation(a, rea__kleene_star(rea__copy(a)));
}

// END rea__kleene_plus
// -----------------------------------------------------------------------------

// END OPERATORS
// =============================================================================
