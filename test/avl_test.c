#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include "libsapling/dm/avl.h"
#include "test/test_utils.h"

static
void print_integer(FILE *stream, const void *data)
{
    fprintf(stream, "%d", *(int *)data);
}

static
int cmpi(const void *a, const void *b)
{
    return *(int *)a <= *(int *)b;
}

static
int equi(const void *a, const void *b)
{
    return *(int *)a == *(int *)b;
}

IMPLEMENT_TYPED_AVL(integer_avl, int, cmpi, equi, print_integer)
DEFINE_OUTPUT_STATE_FUNC(integer_avl)

static
int rand_rand_sign(int m)
{
    return ((rand() & 1) ? -1 : 1) * (rand() % m);
}

struct check_ascending_order {
    int prev;
    int satisf;
};

static
int check_ascending(const int *data, void *info)
{
    struct info_insert *ii = info;
    struct check_ascending_order *user = ii->info;

    int a = user->prev;
    int b = *data;
    int temp = !(a <= b);
    user->prev = *data;
    return temp;
}

static
void set_not_satisf(int *data, void *info)
{
    struct info_insert *ii = info;
    struct check_ascending_order *user = ii->info;

    user->satisf = 0;
}

int main(int argc, char *argv[])
{
    PARSE_OPTIONS()

    srand(time(NULL));

    node_t set = NULL;
    integer_avl__print_data(stdout, &set);

    // insert random values
    int n = 40;
    int m = 100;
    int record[n];
    for (int i = 0; i < n; i++) {
        integer_avl__insert(&set, record[i] = rand_rand_sign(m),
            integer_avl__cmp_predicate);
        integer_avl__output_state(&set);
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

    // check all values in
    // randomly access data and ensure it matches
    for (int i = 0; i < n; i++) {
        assert(integer_avl__in(&set, record[indexes[i]]));
    }
    // ensure inorder
    // ensure order in inorder traversal
    struct check_ascending_order cao = { INT_MIN, 1 };
    integer_avl__access(U_QT, &set, &cao, check_ascending, set_not_satisf);
    assert(cao.satisf);

    // randomly delete
    for (int i = 0; i < n; i++) {
        integer_avl__delete(&set, record[indexes[i]], integer_avl__equ_predicate);
        integer_avl__output_state(&set);
    }
    // assert null
    assert(set == NULL);
}