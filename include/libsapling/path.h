#ifndef PATH
#define PATH

#include "graph.h"

size_t path__edge_storage(void);
void *path__node__data(void **ref);

void path__insert(void **ref, void *info, match_fn match);
void path__delete(int qt, void **ref, void *info, match_fn match);
void path__access(int qt, void **ref, void *info, match_fn match,
    apply_fn apply);

void path__dump_dot(FILE *fd, void **ref, void (*fpd)(FILE *fd, void **ref));

#endif
