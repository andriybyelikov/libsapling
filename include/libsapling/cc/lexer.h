#ifndef LIBSAPLING_LEXER
#define LIBSAPLING_LEXER

#include "libsapling/dm/graph.h"

void *lexer__data(const node_t node);
void lexer__access(enum qt qt, node_t *ref, void *info, predicate_fn predicate,
    apply_fn apply);
void lexer__dump_dot(FILE *stream, node_t *ref, fpfdata_fn fpfdata);

void lexer__literal(node_t *dst, const char *expr);
void lexer__class(node_t *dst, const char *expr);
void lexer__anything(node_t *dst);
void lexer__copy(node_t *dst, node_t *src);

void lexer__union(node_t *dst, node_t *src);
void lexer__concatenation(node_t *dst, node_t *src);
void lexer__kleene_star(node_t *dst);
void lexer__kleene_plus(node_t *dst);

void lexer__minimize(node_t *dst);

struct lexer_state {
    const char *buf;
    int cursor;
    int overread;
    void **ref_to_data;
};

void lexer__set_accepting_states_data(node_t *ref, void *data);
void lexer__init(struct lexer_state *ref, const char *buf);
void *lexer__next_terminal(node_t *ref, struct lexer_state *sta);

extern int libsapling_lexer_explain_minimization;

node_t lexer__enumerate_states(node_t *ref);
const char *lexer__to_string(node_t *ref, node_t *map);

#endif
