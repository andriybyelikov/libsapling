#include <assert.h>
#include <string.h>
#include "rea.h"
#include "text.h"

static
void dump_dot(void **ref, int dump_copies, void (*fpd)(FILE *, void **))
{
    if (dump_copies) {
        void *cp = NULL;
        rea__copy(&cp, ref);
        rea__dump_dot(stdout, &cp, fpd);
    } else {
        rea__dump_dot(stdout, ref, fpd);
    }
}

static
void fpd_int(FILE *fd, void **ref)
{
    void **data_ref = node__data(*ref, rea__edge_storage());
    void *data_ptr = *data_ref;
    int val = *(int *)data_ptr;
    fprintf(fd, "%d", val);
}

int main(int argc, char *argv[])
{
    int dump_dot_flag = argc > 1 && !strcmp(argv[1], "-v");
    int dump_copies = argc > 2 && !strcmp(argv[2], "-c");


    // test builders
    {
        // test literal
        void *a0 = NULL;
        rea__literal(&a0, "\"literal\"");
        if (dump_dot_flag)
            dump_dot(&a0, dump_copies, NULL);
    }
    {
        // test class
        void *a0 = NULL;
        rea__class(&a0, "[xy-z_t-]");
        if (dump_dot_flag)
            dump_dot(&a0, dump_copies, NULL);
    }
    {
        // test anything
        void *a0 = NULL;
        rea__anything(&a0);
        if (dump_dot_flag)
            dump_dot(&a0, dump_copies, NULL);
    }


    // test operators
    {
        // test union of literals without common prefixes
        void *a0 = NULL;
        rea__literal(&a0, "\"wealth\"");
        void *a1 = NULL;
        rea__literal(&a1, "\"beyond\"");
        void *a2 = NULL;
        rea__literal(&a2, "\"measure\"");
        void *a3 = NULL;
        rea__literal(&a3, "\"outlander\"");
        rea__union(&a2, &a3);
        rea__union(&a1, &a2);
        rea__union(&a0, &a1);
        if (dump_dot_flag)
            dump_dot(&a0, dump_copies, NULL);
    }
    {
        // test union of literals with common prefixes
        void *a0 = NULL;
        rea__literal(&a0, "\"dio\"");
        void *a1 = NULL;
        rea__literal(&a1, "\"diocles\"");
        void *a2 = NULL;
        rea__literal(&a2, "\"diocletianus\"");
        void *a3 = NULL;
        rea__literal(&a3, "\"flavius\"");
        void *a4 = NULL;
        rea__literal(&a4, "\"flavian\"");
        void *a5 = NULL;
        rea__literal(&a5, "\"florianus\"");
        rea__union(&a4, &a5);
        rea__union(&a3, &a4);
        rea__union(&a2, &a3);
        rea__union(&a1, &a2);
        rea__union(&a0, &a1);
        if (dump_dot_flag)
            dump_dot(&a0, dump_copies, NULL);
    }
    {
        // test union of classes
        void *a0 = NULL;
        rea__class(&a0, "[a-c]");
        void *a1 = NULL;
        rea__class(&a1, "[x-zt]");
        void *a2 = NULL;
        rea__class(&a2, "[a-f]");
        rea__union(&a1, &a2);
        rea__union(&a0, &a1);
        if (dump_dot_flag)
            dump_dot(&a0, dump_copies, NULL);
    }
    {
        // test union of anything
        void *a0 = NULL;
        rea__anything(&a0);
        void *a1 = NULL;
        rea__anything(&a1);
        void *a2 = NULL;
        rea__anything(&a2);
        rea__union(&a1, &a2);
        rea__union(&a0, &a1);
        if (dump_dot_flag)
            dump_dot(&a0, dump_copies, NULL);
    }
    {
        // test concatenation of literals
        void *a0 = NULL;
        rea__literal(&a0, "\"fus\"");
        void *a1 = NULL;
        rea__literal(&a1, "\"ro\"");
        void *a2 = NULL;
        rea__literal(&a2, "\"dah\"");
        rea__concatenation(&a1, &a2);
        rea__concatenation(&a0, &a1);
        if (dump_dot_flag)
            dump_dot(&a0, dump_copies, NULL);
    }
    {
        // test concatenation of classes
        void *a0 = NULL;
        rea__class(&a0, "[a-c]");
        void *a1 = NULL;
        rea__class(&a1, "[x-zt]");
        void *a2 = NULL;
        rea__class(&a2, "[a-f]");
        rea__concatenation(&a1, &a2);
        rea__concatenation(&a0, &a1);
        if (dump_dot_flag)
            dump_dot(&a0, dump_copies, NULL);
    }
    {
        // test concatenation of anything
        void *a0 = NULL;
        rea__anything(&a0);
        void *a1 = NULL;
        rea__anything(&a1);
        void *a2 = NULL;
        rea__anything(&a2);
        rea__concatenation(&a1, &a2);
        rea__concatenation(&a0, &a1);
        if (dump_dot_flag)
            dump_dot(&a0, dump_copies, NULL);
    }
    {
        // test kleene star of literal
        void *a0 = NULL;
        rea__literal(&a0, "\"literal\"");
        rea__kleene_star(&a0);
        if (dump_dot_flag)
            dump_dot(&a0, dump_copies, NULL);
    }
    {
        // test kleene plus of literal
        void *a0 = NULL;
        rea__literal(&a0, "\"literal\"");
        rea__kleene_plus(&a0);
        if (dump_dot_flag)
            dump_dot(&a0, dump_copies, NULL);
    }
    {
        // test kleene star of class
        void *a0 = NULL;
        rea__class(&a0, "[x-zt]");
        rea__kleene_star(&a0);
        if (dump_dot_flag)
            dump_dot(&a0, dump_copies, NULL);
    }
    {
        // test kleene plus of class
        void *a0 = NULL;
        rea__class(&a0, "[x-zt]");
        rea__kleene_plus(&a0);
        if (dump_dot_flag)
            dump_dot(&a0, dump_copies, NULL);
    }
    {
        // test kleene star of anything
        void *a0 = NULL;
        rea__anything(&a0);
        rea__kleene_star(&a0);
        if (dump_dot_flag)
            dump_dot(&a0, dump_copies, NULL);
    }
    {
        // test kleene plus of anything
        void *a0 = NULL;
        rea__anything(&a0);
        rea__kleene_plus(&a0);
        if (dump_dot_flag)
            dump_dot(&a0, dump_copies, NULL);
    }
    {
        // test concatenation of kleene star and literal
        void *a0 = NULL;
        rea__literal(&a0, "\"literal\"");
        rea__kleene_star(&a0);
        void *a1 = NULL;
        rea__literal(&a1, "\"class\"");
        rea__concatenation(&a0, &a1);
        if (dump_dot_flag)
            dump_dot(&a0, dump_copies, NULL);
    }
    {
        // case of special interest:
        // test concatenation of kleene star and kleene star
        void *a0 = NULL;
        rea__literal(&a0, "\"literal\"");
        rea__kleene_star(&a0);
        void *a1 = NULL;
        rea__literal(&a1, "\"class\"");
        rea__kleene_star(&a1);
        rea__concatenation(&a0, &a1);
        if (dump_dot_flag)
            dump_dot(&a0, dump_copies, NULL);
    }
    {
        // test concatenation of class kleene plus and literal
        void *a0 = NULL;
        rea__class(&a0, "[x-zt]");
        rea__kleene_plus(&a0);
        void *a1 = NULL;
        rea__literal(&a1, "\"class\"");
        rea__concatenation(&a0, &a1);
        if (dump_dot_flag)
            dump_dot(&a0, dump_copies, NULL);
    }


    // test class kleene plus with escaped characters
    {
        char *expr = "[ \\n\\r\\t]";
        char unesc[10];
        text__unescape(unesc, expr);
        char esc[10];
        text__escape(esc, unesc);
        assert(!strcmp(expr, esc));

        void *a0 = NULL;
        rea__class(&a0, unesc);
        rea__kleene_plus(&a0);
        if (dump_dot_flag)
            dump_dot(&a0, dump_copies, NULL);
    }


    // test data and union of accepting states
    {
        // test union of classes all same class 42
        void *a0 = NULL;
        rea__class(&a0, "[A-F]");
        void *a1 = NULL;
        rea__class(&a1, "[a-f]");
        void *a2 = NULL;
        rea__class(&a2, "[0-9]");
        rea__union(&a1, &a2);
        rea__union(&a0, &a1);
        int id = 42;
        rea__set_accepting_states_data(&a0, &id);
        if (dump_dot_flag)
            dump_dot(&a0, dump_copies, fpd_int);
    }
    {
        // test union of classes each in own class 0, 1, 2
        void *a0 = NULL;
        rea__class(&a0, "[A-F]");
        int id0 = 0;
        rea__set_accepting_states_data(&a0, &id0);
        void *a1 = NULL;
        rea__class(&a1, "[a-f]");
        int id1 = 1;
        rea__set_accepting_states_data(&a1, &id1);
        void *a2 = NULL;
        rea__class(&a2, "[0-9]");
        int id2 = 2;
        rea__set_accepting_states_data(&a2, &id2);
        rea__union(&a1, &a2);
        rea__union(&a0, &a1);
        if (dump_dot_flag)
            dump_dot(&a0, dump_copies, fpd_int);


        // test classification
        char bufmem[256];
        strcpy(bufmem, "Fa8");
        struct rea__buffer abuf = { 0, 0, bufmem, NULL };

        rea__access(E_QT, &a0, &abuf, match_0, NULL);
        int res = abuf.data_ref != NULL ? **(int **)abuf.data_ref : -1;
        assert(res == id0); // classify 'F' as class 0

        rea__access(E_QT, &a0, &abuf, match_0, NULL);
        res = abuf.data_ref != NULL ? **(int **)abuf.data_ref : -1;
        assert(res == id1); // classify 'a' as class 1

        rea__access(E_QT, &a0, &abuf, match_0, NULL);
        res = abuf.data_ref != NULL ? **(int **)abuf.data_ref : -1;
        assert(res == id2); // classify '8' as class 2
    }
}
