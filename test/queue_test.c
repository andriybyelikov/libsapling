#include <assert.h>
#include <string.h>
#include "graph.h"
#include "queue.h"

struct info_insert {
    size_t size;
    int data;
};

static
void fpd_int(FILE *fd, void **ref)
{
    int val = *(int *)node__data(*ref, queue__edge_storage());
    fprintf(fd, "%d", val);
}

int main(int argc, char *argv[])
{
    int dump_dot = argc > 1 && !strcmp(argv[1], "-v");

    void *queue = NULL;

    struct info_insert info = { sizeof(int) };
    // push 1..10 and ensure pop in original order
    for (int i = 1; i <= 10; i++) {
        info.data = i;
        queue__insert(&queue, &info);
        if (dump_dot)
            queue__dump_dot(stdout, &queue, fpd_int);
    }
    for (int i = 1; i <= 10; i++) {
        int val = *(int *)queue__access(&queue);
        assert(val == i);
        queue__delete(&queue);
        if (dump_dot)
            queue__dump_dot(stdout, &queue, fpd_int);
    }
    // ensure all deleted
    assert(queue == NULL);
}
