#include <stdio.h>
#include <stddef.h>
#include "rea.h"

int main(int argc, char *argv[])
{
    // test builders
    {
        void *literal = rea__literal("\"start\"");
        rea__dump_dot(stdout, &literal, NULL);
        void *_union = rea__class("[ax-z_-]");
        rea__dump_dot(stdout, &_union, NULL);
        void *anything = rea__anything();
        rea__dump_dot(stdout, &anything, NULL);
    }

    // test union with literals without common prefixes
    {
        void *l0 = rea__literal("\"wealth\"");
        void *l1 = rea__literal("\"beyond\"");
        void *l2 = rea__literal("\"measure\"");
        void *l3 = rea__literal("\"outlander\"");
        void *u0 = rea__union(l0, rea__union(l1, rea__union(l2, l3)));
        rea__dump_dot(stdout, &u0, NULL);
    }

    // test union with literals with common prefixes
    {
        void *l0 = rea__literal("\"dio\"");
        void *l1 = rea__literal("\"diocles\"");
        void *l2 = rea__literal("\"diocletianus\"");
        void *l3 = rea__literal("\"flavius\"");
        void *l4 = rea__literal("\"flavian\"");
        void *l5 = rea__literal("\"florianus\"");
        void *u0 = rea__union(
            rea__union(l0, rea__union(l1, l2)),
            rea__union(l3, rea__union(l4, l5))
        );
        rea__dump_dot(stdout, &u0, NULL);
    }

    // test union of classes
    {
        void *c0 = rea__class("[a-c]");
        void *c1 = rea__class("[x-zt]");
        void *c2 = rea__class("[a-f]");
        void *u0 = rea__union(c0, rea__union(c1, c2));
        rea__dump_dot(stdout, &u0, NULL);
    }

    // test union of anything
    {
        void *a0 = rea__anything();
        void *a1 = rea__anything();
        void *a2 = rea__anything();
        void *u0 = rea__union(a0, rea__union(a1, a2));
        rea__dump_dot(stdout, &u0, NULL);
    }

    // test concatenation with no loops involved
    {
        void *l0 = rea__literal("\"fus\"");
        void *l1 = rea__literal("\"ro\"");
        void *l2 = rea__literal("\"dah\"");
        void *cat0 = rea__concatenation(l0, rea__concatenation(l1, l2));
        rea__dump_dot(stdout, &cat0, NULL);
    }

    // test concatenation of classes with no loops involved
    {
        void *c0 = rea__class("[a-c]");
        void *c1 = rea__class("[x-zt]");
        void *c2 = rea__class("[a-f]");
        void *cat0 = rea__concatenation(c0, rea__concatenation(c1, c2));
        rea__dump_dot(stdout, &cat0, NULL);
    }

    // test concatenation of anything with no loops involved
    {
        void *a0 = rea__anything();
        void *a1 = rea__anything();
        void *a2 = rea__anything();
        void *cat0 = rea__concatenation(a0, rea__concatenation(a1, a2));
        rea__dump_dot(stdout, &cat0, NULL);
    }


    // test kleene star on literal
    {
        void *ks0 = rea__kleene_star(rea__literal("\"literal\""));
        rea__dump_dot(stdout, &ks0, NULL);
    }

    // test kleene star on class
    {
        void *ks0 = rea__kleene_star(rea__class("[abc]"));
        rea__dump_dot(stdout, &ks0, NULL);
    }

    // test kleene star on anything
    {
        void *ks0 = rea__kleene_star(rea__anything());
        rea__dump_dot(stdout, &ks0, NULL);
    }


    // test kleene plus on literal
    {
        void *ks0 = rea__kleene_plus(rea__literal("\"literal\""));
        rea__dump_dot(stdout, &ks0, NULL);
    }

    // test kleene plus on class
    {
        void *ks0 = rea__kleene_plus(rea__class("[abc]"));
        rea__dump_dot(stdout, &ks0, NULL);
    }

    // test kleene plus on anything
    {
        void *ks0 = rea__kleene_plus(rea__anything());
        rea__dump_dot(stdout, &ks0, NULL);
    }


    // test concatenation of kleene star and literal
    {
        void *ks0 = rea__kleene_star(rea__literal("\"does\""));
        void *l0 = rea__literal("\"work\"");
        void *cat0 = rea__concatenation(ks0, l0);
        rea__dump_dot(stdout, &cat0, NULL);
    }

    // test concatenation of literal and kleene star
    {
        void *ks0 = rea__kleene_star(rea__literal("\"does\""));
        void *l0 = rea__literal("\"work\"");
        void *cat0 = rea__concatenation(l0, ks0);
        rea__dump_dot(stdout, &cat0, NULL);
    }


    // test concatenation of kleene star and class
    /*{
        void *ks0 = rea__kleene_star(rea__class("[abc]"));
        rea__dump_dot(stdout, &ks0, NULL);
        void *l0 = rea__literal("\"xyz\"");
        rea__dump_dot(stdout, &l0, NULL);
        void *cat0 = rea__concatenation(ks0, l0);
        rea__dump_dot(stdout, &cat0, NULL);
    }*/

    // test concatenation of literal and kleene star
    /*{
        void *ks0 = rea__kleene_star(rea__class("[abc]"));
        void *l0 = rea__literal("\"work\"");
        void *cat0 = rea__concatenation(l0, ks0);
        rea__dump_dot(stdout, &cat0, NULL);
    }*/
}
