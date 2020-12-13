#ifndef TERMINAL
#define TERMINAL

void *terminal(int id, const char *lx);
int terminal__id(void *tk);
const char *terminal__lexeme(void *tk);

#endif
