#include <string.h>
#include "cc/lexer/lexer_edge_storage.h"
#include "cc/lexer/lexer_attributed_edges.h"
#include "cc/lexer/lexer_insert_state.h"

static
void insert_character(node_t is, node_t as, char c)
{
    *ae__reserve(&is->attributed_edges, c) = as;
}

static
void insert_range(node_t is, node_t as, char a, char b)
{
    if (a > b) {
        char temp = a;
        a = b;
        b = temp;
    }
    for (char c = a; c <= b; c++)
        insert_character(is, as, c);
}

// a small parser is needed in this case for ranges
void lexer__class(node_t *dst, const char *expr)
{
    insert_state(dst, 0);
    node_t is = *dst; // initial state
    node_t as = NULL; // accepting state
    insert_state(&as, 1);

    int s = 0;
    char buf;
    for (int i = 1; i < strlen(expr) - 1; i++) { // trim brackets
        char c = expr[i];
        switch (s) {
        case 0:
            buf = c;
            s = 1;
            break;
        case 1:
            switch (c) {
            case '-':
                s = 2;
                break;
            default:
                insert_character(is, as, buf);
                buf = c;
                s = 1;
                break;
            }
            break;
        case 2:
            insert_range(is, as, buf, c);
            s = 0;
            break;
        }
    }
    switch (s) {
    case 2:
        insert_character(is, as, '-');
        // fallthrough
    case 1:
        insert_character(is, as, buf);
        break;
    }
}
