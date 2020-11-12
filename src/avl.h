#ifndef AVL
#define AVL

#include "graph.h"

size_t avl__edge_storage(void);

typedef int (*choose_fn)(void **ref, void *info);

void avl__insert(void **ref, void *info, choose_fn choose);
void avl__delete(void **ref, void *info, choose_fn choose, match_fn match);
void avl__access(int qt, void **ref, void *info,
    choose_fn choose, match_fn match, apply_fn apply);

void avl__dump_dot(FILE *fd, void **ref, void (*fpd)(FILE *fd, void **ref));

#endif
