#ifndef TRIE
#define TRIE

#include "graph.h"

size_t trie__edge_storage(void);

void trie__insert(void **ref, const char *key, void *info);
void trie__delete(void **ref, const char *key);
void trie__access(int qt, void **ref, const char *key, void *info,
    apply_fn apply);

void trie__dump_dot(FILE *fd, void **ref, void (*fpd)(FILE *, void **));

#endif
