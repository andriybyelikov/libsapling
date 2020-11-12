#include <assert.h>
#include <string.h>
#include "graph.h"
#include "stack.h"

struct info_insert {
    size_t size;
    int data;
};

static
void fpd_int(FILE *fd, void **ref)
{
    int val = *(int *)node__data(*ref, stack__edge_storage());
    fprintf(fd, "%d", val);
}

int main(int argc, char *argv[])
{
    int dump_dot = argc > 1 && !strcmp(argv[1], "-v");

    void *stack = NULL;

    struct info_insert info = { sizeof(int) };
    // push 1..10 and ensure pop in reverse order
    for (int i = 1; i <= 10; i++) {
        info.data = i;
        stack__insert(&stack, &info);
        if (dump_dot)
            stack__dump_dot(stdout, &stack, fpd_int);
    }
    for (int i = 10; i >= 1; i--) {
        int val = *(int *)stack__access(&stack);
        assert(val == i);
        stack__delete(&stack);
        if (dump_dot)
            stack__dump_dot(stdout, &stack, fpd_int);
    }
    // assert all deleted
    assert(stack == NULL);
}
