#include <assert.h>
#include <string.h>
#include "libsapling/cc/parse_tree.h"

#define DUMP(SYMBOL, ALIAS) \
if (flag_dump_dot) { \
    printf(#ALIAS "\n"); \
    parse_tree__dump_dot(stdout, &SYMBOL); \
}

int main(int argc, char *argv[])
{
    int flag_dump_dot = 0;
    for (int i = 1; i < argc; i++)
    if (*argv[i] == '-') {
        switch (*(argv[i] + 1)) {
        case 'g':
            flag_dump_dot = 1;
            break;
        }
    }

    printf("%s\n", argv[0]);

    node_t mul = parse_tree__create_node("*");
    parse_tree__append_child(&mul, parse_tree__create_node("pi"));
    parse_tree__append_child(&mul, parse_tree__create_node("i"));

    node_t pow = parse_tree__create_node("^");
    parse_tree__append_child(&pow, parse_tree__create_node("e"));
    parse_tree__append_child(&pow, mul);

    node_t add = parse_tree__create_node("+");
    parse_tree__append_child(&add, pow);
    parse_tree__append_child(&add, parse_tree__create_node("1"));

    node_t pt = parse_tree__create_node("=");
    parse_tree__append_child(&pt, add);
    parse_tree__append_child(&pt, parse_tree__create_node("0"));

    DUMP(pt, euler_s_identity)

    // assert root is =
    assert(!strcmp(parse_tree__data(pt), "="));

    // search for child of =, 0
    node_t zero_str = parse_tree__get_child_by_string(&pt, "0");
    node_t zero_pos = parse_tree__get_child_by_position(&pt, 1);
    assert(!strcmp(parse_tree__data(zero_str),
        parse_tree__data(zero_pos)));


    // build a regex-like tree
    node_t n0 = parse_tree__create_node("[a-z]");
    node_t n1 = parse_tree__create_node("t_union");
    parse_tree__append_child(&n1, n0);
    node_t n2 = parse_tree__create_node("expr_4");
    parse_tree__append_child(&n2, n1);
    node_t n3 = parse_tree__create_node("expr_1");
    parse_tree__append_child(&n3, n2);
    node_t n4 = parse_tree__create_node("[a-z0-9_]");
    node_t n5 = parse_tree__create_node("t_union");
    parse_tree__append_child(&n5, n4);
    node_t n6 = parse_tree__create_node("expr_4");
    parse_tree__append_child(&n6, n5);
    node_t n7 = parse_tree__create_node("*");
    node_t n8 = parse_tree__create_node("t_kleenestar");
    parse_tree__append_child(&n8, n7);
    node_t n9 = parse_tree__create_node("expr_3");
    parse_tree__append_child(&n9, n6);
    parse_tree__append_child(&n9, n8);
    node_t n10 = parse_tree__create_node("expr_1");
    parse_tree__append_child(&n10, n9);
    node_t n11 = parse_tree__create_node("expr_0");
    parse_tree__append_child(&n11, n10);
    node_t n12 = parse_tree__create_node("expr_0");
    parse_tree__append_child(&n12, n3);
    parse_tree__append_child(&n12, n11);
    node_t n13 = parse_tree__create_node("expr");
    parse_tree__append_child(&n13, n12);

    DUMP(n13, regex)
}
