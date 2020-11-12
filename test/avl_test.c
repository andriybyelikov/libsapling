#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "graph.h"
#include "avl.h"

static
int even(int a)
{
    return a % 2 == 0;
}

struct info_insert {
    size_t size;
    int data;
};

static
int choose(void **ref, void *info)
{
    struct infostack *is = info;
    struct info_insert *i = is->user;
    void *node = *ref;
    int a = i->data;
    int b = *(int *)node__data(node, avl__edge_storage());
    return a < b ? 0 : 1;
}

static
int match(void **ref, void *info)
{
    struct infostack *is = info;
    struct info_insert *i = is->user;
    void *node = *ref;
    int a = i->data;
    int b = *(int *)node__data(node, avl__edge_storage());
    return a == b;
}

static
void fpd_int(FILE *fd, void **ref)
{
    int val = *(int *)node__data(*ref, avl__edge_storage());
    fprintf(fd, "%d", val);
}

static
void check_exists(void **ref, void *info)
{
    if (*ref != NULL) {
        struct infostack *is = info;
        struct info_insert *i = is->user;
        void *node = *ref;
        int a = *(int *)node__data(node, avl__edge_storage());
        i->data = a;
    }
}

int main(int argc, char *argv[])
{
    int dump_dot = argc > 1 && !strcmp(argv[1], "-v");

    void *avl = NULL;

    struct info_insert ii = { sizeof(int) };

    srand(time(NULL));
    const int n = 40;
    const int m = 100;
    int record[n];

    // insert 'n random values mod 'm and save them in record
    for (int i = 0; i < n; i++) {
        record[i] = ii.data = (rand() % m) * (even(rand()) ? 1 : -1);
        avl__insert(&avl, &ii, choose);
        if (dump_dot)
            avl__dump_dot(stdout, &avl, fpd_int);
    }

    // shuffle an array of indexes
    int indexes[n];
    for (int i = 0; i < n; i++)
        indexes[i] = i;
    for (int i = 0; i < m; i++) {
        int i0 = rand() % n;
        int i1 = rand() % n;
        int temp = indexes[i0];
        indexes[i0] = indexes[i1];
        indexes[i1] = temp;
    }

    // randomly access data and ensure it matches
    for (int i = 0; i < n; i++) {
        ii.data = record[indexes[i]];
        avl__access(E_QT, &avl, &ii, choose, match, check_exists);
        assert(ii.data == record[indexes[i]]);
    }

    // randomly delete values
    for (int i = 0; i < n; i++) {
        ii.data = record[indexes[i]];
        avl__delete(&avl, &ii, choose, match);
        if (dump_dot)
            avl__dump_dot(stdout, &avl, fpd_int);
    }

    // assert all deleted
    assert(avl == NULL);
}
