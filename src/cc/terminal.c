#include <stdlib.h>
#include <string.h>
#include "libsapling/cc/terminal.h"

struct terminal {
    int id;
    char *lexeme;
};


terminal_t new_terminal(int id, const char *lexeme)
{
    terminal_t ref = malloc(sizeof(struct terminal));

    ref->id = id;
    ref->lexeme = malloc(strlen(lexeme) + 1), strcpy(ref->lexeme, lexeme);

    return ref;
}

void delete_terminal(terminal_t ref)
{
    free(ref->lexeme);
    free(ref);
}

int terminal__id(terminal_t ref)
{
    return ref->id;
}

const char *terminal__lexeme(terminal_t ref)
{
    return ref->lexeme;
}
