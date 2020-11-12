#ifndef QUEUE
#define QUEUE

#include <stddef.h>
#include <stdio.h>

size_t queue__edge_storage(void);

void queue__insert(void **ref, void *info);
void queue__delete(void **ref);
void *queue__access(void **ref);

void queue__dump_dot(FILE *fd, void **ref, void (*fpd)(FILE *fd, void **ref));

#endif
