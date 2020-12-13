#ifndef PARSE_TREE
#define PARSE_TREE

#include "graph.h"

void *parse_tree__create_node(const char *string);
void parse_tree__append_child(void **ref, void *child);
void parse_tree__dump_dot(FILE *fd, void **ref);

const char *parse_tree__node__get_string(void **ref);
void *parse_tree__get_child_by_string(void **ref, const char *string);
void *parse_tree__get_child_by_position(void **ref, int pos);

#endif
