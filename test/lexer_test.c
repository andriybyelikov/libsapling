#include <assert.h>
#include <string.h>
#include "libsapling/cc/text.h"
#include "libsapling/cc/lexer.h"
#include "test/test_utils.h"

static
void print_integer(FILE *stream, const void *data)
{
    // data is pointer to data, need double dereference
    if (*(void **)data != NULL)
        fprintf(stream, "%d", **(int **)data);
}

#define DUMP(SYMBOL, ALIAS) \
if (flag_dump_dot) { \
    printf(#ALIAS "\n"); \
    lexer__dump_dot(stdout, &SYMBOL, print_integer); \
    if (dump_copies) { \
        node_t copy = NULL; \
        lexer__copy(&copy, &SYMBOL); \
        printf(#ALIAS "_copy\n"); \
        lexer__dump_dot(stdout, &copy, print_integer); \
    }\
    lexer__minimize(&SYMBOL); \
    printf(#ALIAS "_minimized\n"); \
    lexer__dump_dot(stdout, &SYMBOL, print_integer); \
    if (dump_copies) { \
        node_t minimized_copy = NULL; \
        lexer__copy(&minimized_copy, &SYMBOL); \
        printf(#ALIAS "_minimized_copy\n"); \
        lexer__dump_dot(stdout, &minimized_copy, print_integer); \
    }\
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

    int dump_copies = 0;

    printf("%s\n", argv[0]);

    // test literal builder
    {
        node_t a0 = NULL;
        lexer__literal(&a0, "\"literal\"");
        DUMP(a0, literal)
    }

    // test class builder
    {
        node_t a0 = NULL;
        lexer__class(&a0, "[xy-zt_-]");
        DUMP(a0, class)
    }

    // test anything builder
    {
        node_t a0 = NULL;
        lexer__anything(&a0);
        DUMP(a0, anything)
    }

    // test union of literals without common prefixes
    {
        node_t a0 = NULL;
        lexer__literal(&a0, "\"wealth\"");
        node_t a1 = NULL;
        lexer__literal(&a1, "\"beyond\"");
        node_t a2 = NULL;
        lexer__literal(&a2, "\"measure\"");
        node_t a3 = NULL;
        lexer__literal(&a3, "\"outlander\"");

        lexer__union(&a2, &a3);
        lexer__union(&a1, &a2);
        lexer__union(&a0, &a1);

        DUMP(a0, union_literals_only_without_common_prefixes)
    }

    // test union of literals with common prefixes
    {
        node_t a0 = NULL;
        lexer__literal(&a0, "\"dio\"");
        node_t a1 = NULL;
        lexer__literal(&a1, "\"diocles\"");
        node_t a2 = NULL;
        lexer__literal(&a2, "\"diocletianus\"");
        node_t a3 = NULL;
        lexer__literal(&a3, "\"flavius\"");
        node_t a4 = NULL;
        lexer__literal(&a4, "\"flavian\"");
        node_t a5 = NULL;
        lexer__literal(&a5, "\"florianus\"");

        lexer__union(&a4, &a5);
        lexer__union(&a3, &a4);
        lexer__union(&a2, &a3);
        lexer__union(&a1, &a2);
        lexer__union(&a0, &a1);
        DUMP(a0, union_literals_only_with_common_prefixes_a1)
    }

    // test union of classes
    {
        node_t a0 = NULL;
        lexer__class(&a0, "[a-c]");
        node_t a1 = NULL;
        lexer__class(&a1, "[x-zt]");
        node_t a2 = NULL;
        lexer__class(&a2, "[a-f]");

        lexer__union(&a1, &a2);
        lexer__union(&a0, &a1);
        DUMP(a0, union_classes)
    }

    // test union of anything
    {
        node_t a0 = NULL;
        lexer__anything(&a0);
        node_t a1 = NULL;
        lexer__anything(&a1);
        node_t a2 = NULL;
        lexer__anything(&a2);

        lexer__union(&a1, &a2);
        lexer__union(&a0, &a1);
        DUMP(a0, union_anything)
    }

    // test concatenation of literals
    {
        node_t a0 = NULL;
        lexer__literal(&a0, "\"fus\"");
        node_t a1 = NULL;
        lexer__literal(&a1, "\"ro\"");
        node_t a2 = NULL;
        lexer__literal(&a2, "\"dah\"");

        lexer__concatenation(&a1, &a2);
        lexer__concatenation(&a0, &a1);
        DUMP(a0, concatentation_literals_only)
    }

    // test concatenation of classes
    {
        node_t a0 = NULL;
        lexer__class(&a0, "[a-c]");
        node_t a1 = NULL;
        lexer__class(&a1, "[x-zt]");
        node_t a2 = NULL;
        lexer__class(&a2, "[a-f]");

        lexer__concatenation(&a1, &a2);
        lexer__concatenation(&a0, &a1);
        DUMP(a0, concatentation_classes_only)
    }

    // test concatenation of anything
    {
        node_t a0 = NULL;
        lexer__anything(&a0);
        node_t a1 = NULL;
        lexer__anything(&a1);
        node_t a2 = NULL;
        lexer__anything(&a2);

        lexer__concatenation(&a1, &a2);
        lexer__concatenation(&a0, &a1);
        DUMP(a0, concatentation_anything_only)
    }

    // test kleene star of literal
    {
        node_t a0 = NULL;
        lexer__literal(&a0, "\"literal\"");

        lexer__kleene_star(&a0);
        DUMP(a0, kleene_star_literal_only)
    }

    // test kleene plus of literal
    {
        node_t a0 = NULL;
        lexer__literal(&a0, "\"literal\"");

        lexer__kleene_plus(&a0);
        DUMP(a0, kleene_plus_literal_only)
    }

    // test kleene star of class
    {
        node_t a0 = NULL;
        lexer__class(&a0, "[x-zt]");

        lexer__kleene_star(&a0);
        DUMP(a0, kleene_star_class_only)
    }

    // test kleene plus of class
    {
        node_t a0 = NULL;
        lexer__class(&a0, "[x-zt]");

        lexer__kleene_plus(&a0);
        DUMP(a0, kleene_plus_class_only)
    }

    // test kleene star of anything
    {
        node_t a0 = NULL;
        lexer__anything(&a0);

        lexer__kleene_star(&a0);
        DUMP(a0, kleene_star_anything_only)
    }

    // test kleene plus of anything
    {
        node_t a0 = NULL;
        lexer__anything(&a0);

        lexer__kleene_plus(&a0);
        DUMP(a0, kleene_plus_anything_only)
    }

    // test concatenation of kleene star and literal
    {
        node_t a0 = NULL;
        lexer__literal(&a0, "\"literal\"");
        lexer__kleene_star(&a0);

        node_t a1 = NULL;
        lexer__literal(&a1, "\"class\"");

        lexer__concatenation(&a0, &a1);
        DUMP(a0, concatenation_kleene_star_and_literal)
    }

    // CASE OF SPECIAL INTEREST
    // test concatenation of kleene star and kleene star
    {
        node_t a0 = NULL;
        lexer__literal(&a0, "\"literal\"");
        lexer__kleene_star(&a0);

        node_t a1 = NULL;
        lexer__literal(&a1, "\"class\"");
        lexer__kleene_star(&a1);

        lexer__concatenation(&a0, &a1);
        DUMP(a0, concatenation_kleene_star_and_kleene_star)
    }

    // test concatenation of class kleene plus and literal
    {
        node_t a0 = NULL;
        lexer__class(&a0, "[x-zt]");
        lexer__kleene_plus(&a0);

        node_t a1 = NULL;
        lexer__literal(&a1, "\"class\"");

        lexer__concatenation(&a0, &a1);
        DUMP(a0, concatenation_kleene_plus_and_literal)
    }

    // test class kleene plus with escaped characters
    {
        const char *expr = "[ \\n\\r\\t]";
        char unesc[10];
        text__unescape(unesc, expr);
        char esc[10];
        text__escape(esc, unesc);
        assert(!strcmp(expr, esc));

        node_t a0 = NULL;
        lexer__class(&a0, unesc);
        lexer__kleene_plus(&a0);

        DUMP(a0, kleene_plus_escaped_characters)
    }

    // test data and union of accepting states
    {
        // test union of classes all same class 42
        node_t a0 = NULL;
        lexer__class(&a0, "[A-F]");
        node_t a1 = NULL;
        lexer__class(&a1, "[a-f]");
        node_t a2 = NULL;
        lexer__class(&a2, "[0-9]");

        lexer__union(&a1, &a2);
        lexer__union(&a0, &a1);

        int id = 42;
        lexer__set_accepting_states_data(&a0, &id);

        DUMP(a0, union_class_42)
    }

    // test union of classes each in own class 0, 1, 2
    {
        node_t a0 = NULL;
        lexer__class(&a0, "[A-F]");
        int id0 = 0;
        lexer__set_accepting_states_data(&a0, &id0);
        node_t a1 = NULL;
        lexer__class(&a1, "[a-f]");
        int id1 = 1;
        lexer__set_accepting_states_data(&a1, &id1);
        node_t a2 = NULL;
        lexer__class(&a2, "[0-9]");
        int id2 = 2;
        lexer__set_accepting_states_data(&a2, &id2);

        lexer__union(&a1, &a2);
        lexer__union(&a0, &a1);

        DUMP(a0, union_classes_1_2_3)


        // test classification
        char bufmem[256];
        strcpy(bufmem, "Fa8");
        struct lexer_state lsta;
        lexer__init(&lsta, bufmem);
        int *res;

        res = (int *)lexer__next_terminal(&a0, &lsta);
        assert(res != NULL && *res == id0); // classify 'F' as class 0

        res = (int *)lexer__next_terminal(&a0, &lsta);
        assert(res != NULL && *res == id1); // classify 'a' as class 1

        res = (int *)lexer__next_terminal(&a0, &lsta);
        assert(res != NULL && *res == id2); // classify '8' as class 2
    }
}
