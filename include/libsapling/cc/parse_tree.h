#ifndef _LIBSAPLING_PARSE_TREE_H_
#define _LIBSAPLING_PARSE_TREE_H_

/**
 * @file parse_tree.h
 * @brief Parse tree implementation
 */

#include "libsapling/dm/graph.h"

const char **parse_tree__data(node_t node);

node_t parse_tree__create_node(char *string);
void parse_tree__append_child(node_t *ref, node_t child);

node_t parse_tree__get_child_by_position(node_t *ref, int pos);
node_t parse_tree__get_child_by_string(node_t *ref, const char *str);

void parse_tree__dump_dot(FILE *stream, node_t *ref);

#endif
