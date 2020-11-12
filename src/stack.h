#ifndef STACK
#define STACK

#include <stddef.h>
#include <stdio.h>

size_t stack__edge_storage(void);

void stack__insert(void **ref, void *info);
void stack__delete(void **ref);
void *stack__access(void **ref);

void stack__dump_dot(FILE *fd, void **ref, void (*fpd)(FILE *fd, void **ref));

#endif
