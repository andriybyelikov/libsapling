#ifndef GRAPH
#define GRAPH

#include <stddef.h> // size_t
#include <stdio.h>  // FILE

// quantifiers
enum {
    U_QT, // universal quantifier
    E_QT  // existential quantifier
};


typedef int (*match_fn)(void **ref, void *info);
// Function that decides, in a universal loop, whether to apply the apply
// function on the current node, or, in an existential loop, whether to stop
// the loop at the current node and apply the apply function on it.
// Trivial and useful match functions:
int match_0(void **ref_unused, void *info_unused); // always returns 0
int match_1(void **ref_unused, void *info_unused); // always returns 1

typedef void (*next_fn)(void ***ref, void *info);
// Chooses the next node to explore.

typedef void (*apply_fn)(void **ref, void *info);
// Performs a user defined operation on the node.


void graph__uloop(void **ref, void *info,
    match_fn match, next_fn next, apply_fn apply);
// Iterates through all the nodes of the graph.
// Equivalent of traversal and the universal quantifier.

void graph__eloop(void **ref, void *info,
    match_fn match, next_fn next, apply_fn apply);
// Iterates through certain nodes of the graph until it reaches a target node.
// Equivalent of search and the existential quantifier.


struct edge_storage;

typedef void (*edge_management_fn)(void **ref, struct edge_storage *node,
    void *info);
// Performs edge management during insertion/deletion.
// On insertion *ref is the edge that should somehow end up connecting the
// newly created node to the rest of the graph.
// On deletion *ref is the edge that connects the node to be deleted with the
// rest of the graph.

void graph__insert(void **ref, void *info,
    size_t edge_storage, edge_management_fn edge_management);
// Utility function that performs common tasks of node insertion plus specific
// edge management.

void graph__delete(void **ref, void *info,
    edge_management_fn edge_management);
// Utility function that performs common tasks of node deletion plus specific
// edge management.


void *node__data(void *ref, size_t edge_storage);
// Returns a pointer to where the node's data should be.


// info utils

struct infostack {
    void *user;
    void *impl;
};

void *get_user_info(void *info);
void *get_impl_info(void *info);

struct info_dump_dot {
    FILE *fd;
    void (*fpd)(FILE *, void **);
};

#endif
