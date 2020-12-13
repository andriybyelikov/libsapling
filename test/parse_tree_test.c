#include <assert.h>
#include <string.h>
#include "parse_tree.h"

int main(int argc, char *argv[])
{
    int dump_dot = argc > 1 && !strcmp(argv[1], "-v");


    void *mul = parse_tree__create_node("*");
    parse_tree__append_child(&mul, parse_tree__create_node("pi"));
    parse_tree__append_child(&mul, parse_tree__create_node("i"));

    void *pow = parse_tree__create_node("^");
    parse_tree__append_child(&pow, parse_tree__create_node("e"));
    parse_tree__append_child(&pow, mul);

    void *add = parse_tree__create_node("+");
    parse_tree__append_child(&add, pow);
    parse_tree__append_child(&add, parse_tree__create_node("1"));

    void *pt = parse_tree__create_node("=");
    parse_tree__append_child(&pt, add);
    parse_tree__append_child(&pt, parse_tree__create_node("0"));

    assert(!strcmp(parse_tree__node__get_string(&pt), "="));

    // search for child of =, 0
    void *zero_str = parse_tree__get_child_by_string(&pt, "0");
    void *zero_pos = parse_tree__get_child_by_position(&pt, 1);
    assert(!strcmp(parse_tree__node__get_string(&zero_str),
        parse_tree__node__get_string(&zero_pos)));

    if (dump_dot)
        parse_tree__dump_dot(stdout, &pt);


    // build a regex-like tree
    void *n0 = parse_tree__create_node("[a-z]");
    void *n1 = parse_tree__create_node("t_union");
    parse_tree__append_child(&n1, n0);
    void *n2 = parse_tree__create_node("expr_4");
    parse_tree__append_child(&n2, n1);
    void *n3 = parse_tree__create_node("expr_1");
    parse_tree__append_child(&n3, n2);
    void *n4 = parse_tree__create_node("[a-z0-9_]");
    void *n5 = parse_tree__create_node("t_union");
    parse_tree__append_child(&n5, n4);
    void *n6 = parse_tree__create_node("expr_4");
    parse_tree__append_child(&n6, n5);
    void *n7 = parse_tree__create_node("*");
    void *n8 = parse_tree__create_node("t_kleenestar");
    parse_tree__append_child(&n8, n7);
    void *n9 = parse_tree__create_node("expr_3");
    parse_tree__append_child(&n9, n6);
    parse_tree__append_child(&n9, n8);
    void *n10 = parse_tree__create_node("expr_1");
    parse_tree__append_child(&n10, n9);
    void *n11 = parse_tree__create_node("expr_0");
    parse_tree__append_child(&n11, n10);
    void *n12 = parse_tree__create_node("expr_0");
    parse_tree__append_child(&n12, n3);
    parse_tree__append_child(&n12, n11);
    void *n13 = parse_tree__create_node("expr");
    parse_tree__append_child(&n13, n12);

    if (dump_dot)
        parse_tree__dump_dot(stdout, &n13);
}
