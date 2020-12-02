#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "graph.h"
#include "path.h"


static
int asc(int a, int b)
{
    return a <= b;
}

static
int desc(int a, int b)
{
    return a >= b;
}

static
int even(int a)
{
    return a % 2 == 0;
}

static
int odd(int a)
{
    return a % 2 != 0;
}


struct info_insert {
    size_t size;
    int data;
    int (*x)(int, int);
};

static
int insert_xending(void **ref, void *info)
{
    struct info_insert *i = get_user_info(info);
    void *node = *ref;
    int a = i->data;
    int b = *(int *)path__node__data(ref);
    return i->x(a, b);
}

static
int match_last(void **ref, void *info)
{
    return **(void ***)ref == NULL;
}

struct info_match_parity {
    int (*x)(int);
};

static
int match_parity(void **ref, void *info)
{
    struct info_match_parity *i = get_user_info(info);
    void *node = *ref;
    int a = *(int *)path__node__data(ref);
    return i->x(a);
}


struct info_check_xending {
    int satisf;
    int prev;
    int (*x)(int, int);
};

static
int check_xending(void **ref, void *info)
{
    struct info_check_xending *i = get_user_info(info);
    void *node = *ref;
    int a = i->prev;
    int b = *(int *)path__node__data(ref);
    i->prev = b;
    return !(i->satisf = i->x(a, b));
}


struct info_check_parity {
    int satisf;
    int (*x)(int);
};

static
int check_parity(void **ref, void *info)
{
    struct info_check_parity *i = get_user_info(info);
    void *node = *ref;
    int a = *(int *)path__node__data(ref);
    return !(i->satisf = i->x(a));
}


static
void fpd_int(FILE *fd, void **ref)
{
    int val = *(int *)path__node__data(ref);
    fprintf(fd, "%d", val);
}


int main(int argc, char *argv[])
{
    int dump_dot = argc > 1 && !strcmp(argv[1], "-v");

    void *al = NULL;
    void *dl = NULL;

    // insert 'n random values mod 'm
    const int n = 10;
    const int m = 100;
    srand(time(NULL));
    struct info_insert ii = { sizeof(int) };

    ii.x = asc;
    for (int i = 0; i < n; i++) {
        ii.data = (rand() % m) * (even(rand()) ? 1 : -1);
        path__insert(&al, &ii, insert_xending);
        if (dump_dot)
            path__dump_dot(stdout, &al, fpd_int);
    }

    ii.x = desc;
    for (int i = 0; i < n; i++) {
        ii.data = (rand() % m) * (even(rand()) ? 1 : -1);
        path__insert(&dl, &ii, insert_xending);
        if (dump_dot)
            path__dump_dot(stdout, &dl, fpd_int);
    }

    // check that xending order is satisfied
    struct info_check_xending icx;

    icx = (struct info_check_xending){ 0, INT_MIN, asc };
    path__access(E_QT, &al, &icx, check_xending, NULL);
    assert(icx.satisf);

    icx = (struct info_check_xending){ 0, INT_MAX, desc };
    path__access(E_QT, &dl, &icx, check_xending, NULL);
    assert(icx.satisf);

    // insert values that break the order at the beginning and the end of the
    // path and make sure that xending order is not satisfied
    ii.x = asc;

    ii.data = INT_MAX;
    path__insert(&al, &ii, match_1);
    icx = (struct info_check_xending){ 0, INT_MIN, asc };
    path__access(E_QT, &al, &icx, check_xending, NULL);
    assert(!icx.satisf);
    path__delete(E_QT, &al, NULL, match_1);

    ii.data = INT_MIN;
    path__insert(&al, &ii, match_0);
    icx = (struct info_check_xending){ 0, INT_MIN, asc };
    path__access(E_QT, &al, &icx, check_xending, NULL);
    assert(!icx.satisf);
    path__delete(E_QT, &al, NULL, match_last);


    ii.x = desc;

    ii.data = INT_MAX;
    path__insert(&dl, &ii, match_1);
    icx = (struct info_check_xending){ 0, INT_MIN, desc };
    path__access(E_QT, &dl, &icx, check_xending, NULL);
    assert(!icx.satisf);
    path__delete(E_QT, &dl, NULL, match_1);

    ii.data = INT_MIN;
    path__insert(&dl, &ii, match_0);
    icx = (struct info_check_xending){ 0, INT_MIN, desc };
    path__access(E_QT, &dl, &icx, check_xending, NULL);
    assert(!icx.satisf);
    path__delete(E_QT, &dl, NULL, match_last);

    // delete all odd from ascending list, delete all even from descending list
    struct info_match_parity imp;
    imp.x = odd;
    path__delete(U_QT, &al, &imp, match_parity);
    if (dump_dot)
        path__dump_dot(stdout, &al, fpd_int);
    imp.x = even;
    path__delete(U_QT, &dl, &imp, match_parity);
    if (dump_dot)
        path__dump_dot(stdout, &dl, fpd_int);

    // check parity of all
    struct info_check_parity icp;
    icp = (struct info_check_parity){ 0, even };
    path__access(E_QT, &al, &icp, check_parity, NULL);
    assert(icp.satisf);
    icp = (struct info_check_parity){ 0, odd };
    path__access(E_QT, &dl, &icp, check_parity, NULL);
    assert(icp.satisf);

    // insert random parity breaking value and check that parity is not for all
    ii.x = asc;
    ii.data = (rand() % m) * (even(rand()) ? 1 : -1);
    ii.data += even(ii.data) ? 1 : 0;
    path__insert(&al, &ii, insert_xending);
    icp = (struct info_check_parity){ 0, even };
    path__access(E_QT, &al, &icp, check_parity, NULL);
    assert(!icp.satisf);

    ii.x = desc;
    ii.data = (rand() % m) * (even(rand()) ? 1 : -1);
    ii.data += odd(ii.data) ? 1 : 0;
    path__insert(&dl, &ii, insert_xending);
    icp = (struct info_check_parity){ 0, odd };
    path__access(E_QT, &dl, &icp, check_parity, NULL);
    assert(!icp.satisf);

    // delete all and assert that al and dl are NULL
    path__delete(U_QT, &al, NULL, match_1);
    if (dump_dot)
        path__dump_dot(stdout, &al, fpd_int);
    path__delete(U_QT, &dl, NULL, match_1);
    if (dump_dot)
        path__dump_dot(stdout, &dl, fpd_int);
    assert(al == NULL);
    assert(dl == NULL);
}
