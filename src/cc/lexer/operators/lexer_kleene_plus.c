#include "libsapling/cc/lexer.h"
#include "cc/lexer/lexer_minimize.h"

void lexer__kleene_plus(node_t *dst)
{
    node_t copy = NULL;
    lexer__copy(&copy, dst);
    lexer__kleene_star(&copy);
    lexer__concatenation(dst, &copy);
}
