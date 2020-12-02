#ifndef REA
#define REA

#include "graph.h"

size_t rea__edge_storage(void);
void *rea__node__data(void **ref);

void rea__access(int qt, void **ref, void *info,
    match_fn match, apply_fn apply);

void rea__dump_dot(FILE *fd, void **ref, void (*fpd)(FILE *, void **));

// builders
void rea__literal(void **dst, const char *lexeme);
void rea__class(void **dst, const char *lexeme);
void rea__anything(void **dst);
void rea__copy(void **dst, void **src);

// operators
void rea__union(void **dst, void **src);
void rea__concatenation(void **dst, void **src);
void rea__kleene_star(void **dst);
void rea__kleene_plus(void **dst);


// usage
void rea__set_accepting_states_data(void **ref, void *data);

struct rea__buffer {
    int cursor;
    int overread;
    char *mem;
    void **data_ref;
};

#endif
