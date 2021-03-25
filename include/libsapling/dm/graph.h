#ifndef LIBSAPLING_GRAPH
#define LIBSAPLING_GRAPH

/**
 * @file graph.h
 * @brief Graph implementation
 * 
 * This implementation serves as the basis for implementing other dynamic data
 * structures (DDS). The general idea behind this implementation is to
 * represent a set using a graph, taking advantage of the graph's structure to
 * speed up operations, like all data structures aim to do. The hope is that
 * someday it will be possible to automatically determine and implement an
 * optimal graph structure for a set only by analyzing its actual usage in a
 * program and using this implementation as the basis. For now it serves as an
 * abstraction to implement them manually. Implementation for binary set
 * operations is currently neither provided nor the focus, instead the focus of
 * this implementation lies in providing first-order logic data manipulation.
 * For this purpose it provides three elemental operations: insertion, deletion
 * and access, the two latter accepting first-order logic quantifiers, since
 * they manipulate what is already in the set. Insertion is regarded as being
 * existentially quantified, since it searches for the most appropiate location
 * in the graph's structure where a new node must be appended to the overall
 * graph, perhaps according to some attribute of the data to be inserted in the
 * set. Aside from that, some convenience and debugging functions are also
 * provided.
 * 
 * Inspired by the implementation of the C style null-terminated string, which
 * defines a string as a pointer to the first character, this implementation
 * defines a graph as an edge (pointer) to a starting node. Whereas in a string
 * the next character can be accessed by incrementing the pointer by the size
 * of a character, in this graph implementation each node stores the edges that
 * sprout from it to other nodes, referred to as the <b>edge storage</b> of the
 * node. This may be a just an edge to the next node, or several of them, even
 * edge management information such as the balance factor of an AVL node, which
 * is not an edge in itself, it may be a dynamic number of edges with the
 * number of edges stored at the beginning of the storage, it may be another
 * dynamic data structure (another graph), which may store attributed edges
 * sorted by that attribute... In conclusion: as long as the particular
 * implementations can interpret the contents of this storage anything goes.
 * 
 * The edge storage may be followed by a <b>data storage</b>, which combined
 * account for the whole contiguous block of memory allocated at the node's
 * address. Like with the edge storage, anything can go in the data storage as
 * long as the user can interpret the contents. Edge management data must no be
 * mixed with the data storage, edge management information need not be edges
 * exclusively. Particular implementations may, however, appropiate the data
 * storage away from the user, and impose their own interfaces to interact with
 * the data if the use case warrants it. For example, in a lexer graph,
 * accepting states might store an identifier of a terminal class as a pointer
 * to an integer, and decide that a state is not an accepting state if the
 * value of that pointer is NULL.
 * 
 * An important thing to keep in mind is that all defined operations and
 * function prototypes actually accept an edge (pointer) that leads to the
 * node, rather than the node itself, as this simplifies the implementation of
 * insertion and deletion. For example, upon deleting an AVL node with only one
 * child it is sufficient to promote the child to the parent's position by
 * dereferencing the edge and replacing the parent node's address with that of
 * the child.
 */

#include <stddef.h>
#include <stdio.h>
#include "libsapling/idiom.h"
#include "libsapling/dm/logic.h"

typedef struct edge_storage *node_t;

/**
 * @brief Means of passing user and implementation information between function
 * calls.
 */
struct info_stack {
    void *user;
    void *impl;
};

/**
 * @brief Decides whether to run the apply function on an item. Behavior is
 * modified by quantifiers.
 * 
 * When modified with universal quantification the predicate decides whether to
 * run the apply function on the current node.
 * 
 * When modified with existential quantification if the predicate is satisfied
 * then the apply function is run on the current node and the traversal ends.
 */
typedef int (*predicate_fn)
    (const node_t *ref, const struct info_stack *info);

/**
 * @brief Performs a user-defined operation on the node.
 */
typedef void (*apply_fn)(node_t *ref, const struct info_stack *info);

/**
 * @brief Chooses the next node to visit.
 */
typedef int (*next_fn)(node_t **ref, const struct info_stack *info);

/**
 * @brief Called upon the insertion and deletion protocols.
 * 
 * On insertion \c node contains the address of the newly created node and \c
 * ref the position in the graph where it was determined for it to be inserted,
 * so usually edge management functions should assign `*ref = node` at some
 * point. This is however, not enforced with any language construct to account
 * for the case of the AVL tree rotations, where instead of the newly created
 * node being directly appended to the graph at \c ref, it may become a child
 * of its child and the former child is instead appended at \c ref.
 * 
 * On deletion \c node is the same as \c *ref. Likewise it takes care of
 * restructuring the graph at \c ref upon deleting a node.
 * 
 * It also may perform the task of initializing the data of the edge storage.
 * 
 * @attention The insertion and deletion protocols take care of allocating and
 * freeing the \c node, so that task must not be performed within the edge
 * management function.
 */
typedef void (*edge_management_fn)(node_t *ref, const node_t node,
    void *impl_ptr);

/**
 * @brief File print format data function prototype.
 */
typedef void (*fpfdata_fn)(FILE *stream, const void *data);

/**
 * @brief File print format node function prototype.
 */
typedef void (*fpfnode_fn)(FILE *stream, const node_t node,
    fpfdata_fn fpfdata);

/**
 * @brief Generic access all nodes adapter function prototype.
 * 
 * \remark Originally created for use with the generic graph printing functions
 * which invokes the particular DDS' implementation's access function without
 * having to know the implementation of the next function.
 */
typedef void (*all_access_adapter_fn)(node_t *ref, void *info, apply_fn apply);

/**
 * @brief Trivial predicate function that always returns 0.
 * \retval 0
 */
int predicate_0(UNUSED const node_t *ref,
    UNUSED const struct info_stack *info);

/**
 * @brief Trivial predicate function that always returns 1.
 * \retval 1
 */
int predicate_1(UNUSED const node_t *ref,
    UNUSED const struct info_stack *info);

/**
 * @brief Returns a pointer to the data storage of a node.
 * \returns a pointer to the data storage of a node.
 */
void *graph__data(const node_t node, size_t edge_storage);

/**
 * @brief Access protocol. Traverses the graph according to a quantifier
 * modifier and specific implementation workings.
 * 
 * Visits all the reachable nodes starting from the specified node.
 * Equivalent to traversal and associated with the universal quantifier.
 * 
 * Visits the minimum number of nodes in the graph until reaching a target node.
 * Equivalent to search and associated with the existential quantifier.
 */
void graph__access(enum qt qt, node_t *ref, const struct info_stack *info,
    predicate_fn predicate, apply_fn apply, next_fn next);

struct info_insert {
    size_t size;
    void *data;
    void *info;
    int (*predicate_adapter)();
    void (*apply_adapter)();
};

/**
 * @brief Insertion protocol
 * 
 * @attention The protocol expects the contents of the user information of the
 * @ref info_stack to be a \c size_t indicating the size of the data followed
 * by that number of bytes of data. That size will be allocated for the data
 * storage of the node and the data copied over as is. Extra information may
 * follow.
 */
void graph__insert(node_t *ref, const struct info_stack *info,
    edge_management_fn edge_management, size_t edge_storage);


/**
 * @brief Deletion protocol
 * 
 * @attention Deleting a node will also deallocate the memory previously
 * allocated for its data so make sure to keep track of dangling pointers to
 * data storage.
 */
void graph__delete(node_t *ref, const struct info_stack *info,
    edge_management_fn edge_management);

/**
 * @brief Prints all the data contained in the set.
 */
void graph__print_data(FILE *stream, node_t *ref,
    all_access_adapter_fn access_adapter, fpfnode_fn fpfnode,
    fpfdata_fn fpfdata);

/**
 * @brief Dumps the graph in the DOT graph description language.
 */
void graph__dump_dot(FILE *stream, node_t *ref,
    all_access_adapter_fn access_adapter, fpfnode_fn fpfnode,
    fpfdata_fn fpfdata);

/**
 * @brief Returns the number of items contained in the set.
 * @returns the number of items contained in the set.
 */
int graph__length(node_t *ref, all_access_adapter_fn access_adapter);

#endif
