#ifndef REA
#define REA

#include "graph.h"

size_t rea__edge_storage(void);

void rea__access(int qt, void **ref, void *info,
    match_fn match, apply_fn apply);

void rea__dump_dot(FILE *fd, void **ref, void (*fpd)(FILE *, void **));

void *rea__copy(void *a);

void *rea__literal(const char *lexeme);
void *rea__class(const char *lexeme);
void *rea__anything(void);

void *rea__union(void *a0, void *a1);
void *rea__concatenation(void *a0, void *a1);
void *rea__kleene_star(void *a);
void *rea__kleene_plus(void *a);

#endif
