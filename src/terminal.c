#include <stdlib.h>
#include <string.h>

static
int *terminal__id_ptr(void *tk)
{
    return (int *)tk;
}

static
char *terminal__lexeme_ptr(void *tk)
{
    return (char *)tk + sizeof(int);
}


void *terminal(int id, const char *lx)
{
    void *tk = malloc(sizeof(int) + strlen(lx) + 1);
    *terminal__id_ptr(tk) = id;
    strcpy(terminal__lexeme_ptr(tk), lx);
    return tk;
}

int terminal__id(void *tk)
{
    return *terminal__id_ptr(tk);
}

const char *terminal__lexeme(void *tk)
{
    return terminal__lexeme_ptr(tk);
}
