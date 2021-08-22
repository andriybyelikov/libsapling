#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "libsapling/idiom.h"
#include "libsapling/dm/path.h"
#include "libsapling/dm/typed/typed_common.h"
#include "test/test_utils.h"

IMPLEMENT_TYPED_PATH(integer_path, int, fpfdata_int, dummy_cmp)
DEFINE_OUTPUT_STATE_FUNC(integer_path)

static
int ascending_predicate(const int *data, void *info)
{
    struct info_insert *user = info;

    const int a = *(int *)user->data;
    const int b = *data;
    return a <= b;
}

static
int odd_predicate(const int *data, UNUSED void *info)
{
    return abs(*data % 2) == 1;
}

struct check_ascending_order {
    int prev;
    int satisf;
};

static
int check_ascending(const int *data, void *info)
{
    CAST_USER_INFO(struct check_ascending_order *, user, info);

    int a = user->prev;
    int b = *data;
    int temp = !(a <= b);
    user->prev = *data;
    return temp;
}

static
void set_not_satisf(int *data, void *info)
{
    CAST_USER_INFO(struct check_ascending_order *, user, info);

    user->satisf = 0;
}

static
int gen_rand_val_rand_sign_abs_bound(int m)
{
    return ((rand() & 1) ? -1 : 1) * (rand() % m);
}

int main(int argc, char *argv[])
{
    TEST_PARSE_OPTIONS()

    srand(time(NULL));

    node_t ascending_list = NULL;
    integer_path__output_state(&ascending_list);

    // insert random values
    int n = 10;
    int m = 100;
    for (int i = 0; i < n; i++) {
        integer_path__insert(&ascending_list,
            gen_rand_val_rand_sign_abs_bound(m), ascending_predicate);
        integer_path__output_state(&ascending_list);
    }

    // assert length
    assert(integer_path__length(&ascending_list) == n);

    // assert ascending order
    struct check_ascending_order cao = { INT_MIN, 1 };
    integer_path__access(E_QT, &ascending_list, &cao, check_ascending,
        set_not_satisf);
    assert(cao.satisf);

    // insert breaking value, assert not ascending order
    integer_path__insert(&ascending_list, INT_MIN, integer_path__predicate_0);
    integer_path__output_state(&ascending_list);
    cao.prev = INT_MIN;
    integer_path__access(E_QT, &ascending_list, &cao, check_ascending,
        set_not_satisf);
    assert(!cao.satisf);

    // delete odd
    integer_path__delete(U_QT, &ascending_list, NULL, odd_predicate);
    integer_path__output_state(&ascending_list);

    // assert all even
    cao.satisf = 1;
    integer_path__access(E_QT, &ascending_list, &cao, odd_predicate,
        set_not_satisf);
    assert(cao.satisf);

    // insert odd, assert not all even
    integer_path__insert(&ascending_list, 1, integer_path__predicate_1);
    integer_path__output_state(&ascending_list);
    integer_path__access(E_QT, &ascending_list, &cao, odd_predicate,
        set_not_satisf);
    assert(!cao.satisf);
    
    // delete all
    integer_path__delete(U_QT, &ascending_list, NULL,
        integer_path__predicate_1);
    integer_path__output_state(&ascending_list);

    // assert length
    assert(integer_path__length(&ascending_list) == 0);

    // assert null
    assert(ascending_list == NULL);
}
