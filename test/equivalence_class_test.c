#include <assert.h>
#include "libsapling/idiom.h"
#include "libsapling/dm/equivalence_class.h"
#include "libsapling/dm/typed/typed_common.h"

static
int equi(const void *a, const void *b)
{
    return *(int *)a == *(int *)b;
}

IMPLEMENT_TYPED_EQUIVALENCE_CLASS(equivalent_states, int, fpfdata_int, equi)


struct i_invalidate_optimized_states {
    int *optimized_states;
};

static
void invalidate_optimized_states(int *data, void *info)
{
    CAST_USER_INFO(struct i_invalidate_optimized_states *, i2, info);

    i2->optimized_states[*data] = 1;
}


// debug
static
void print_table(int **table, int *optimized_states, int num_states,
    int alphabet_length)
{
    for (int i = 0; i < num_states; i++) {
        if (!optimized_states[i]) {
            for (int j = 0; j < alphabet_length + 1; j++) {
                printf("%d ", *((int *)table + i * (alphabet_length + 1) + j));
            }
            printf("\n");
        }
    }
}

static
int cmp_table(int **output, int **table, int *optimized_states, int num_states,
    int alphabet_length)
{
    int ret = 1;
    int x = 0;
    for (int i = 0; i < num_states; i++) {
        if (!optimized_states[i]) {
            for (int j = 0; j < alphabet_length + 1; j++) {
                int idx = i * (alphabet_length + 1) + j;
                int idx2 = x * (alphabet_length + 1) + j;
                if (*((int *)table + idx) != *((int *)output + idx2)) {
                    ret = 0;
                    break;
                }
            }
            x++;
        }
    }
    return ret;
}

/*
 * The test consists of solving optimization for the deterministic finite
 * automaton that accepts the expression (b|c)d .
 * 
 *        b         d
 *       --->( s1 )--->(( s3 ))
 *      /
 * ->( s0 )
 *      \ c         d
 *       --->( s2 )--->(( s4 ))
 * 
 */
int main(int argc, char *argv[])
{
    int table[][4] = {
    //    b  c  d  acc
        { 1, 2, 5, 0 },
        { 5, 5, 3, 0 },
        { 5, 5, 4, 0 },
        { 5, 5, 5, 1 },
        { 5, 5, 5, 1 },
        { 5, 5, 5, 0 } // sink
    };
    // output should be
    int output[][4] = {
        { 1, 1, 5, 0 },
        { 5, 5, 3, 0 },
        { 5, 5, 5, 1 },
        { 5, 5, 5, 0 }
    };

    const int num_states = sizeof(table) / sizeof(table[0]);
    const int alphabet_length = sizeof(table[0]) / sizeof(int) - 1;

    int optimized_states[num_states];
    for (int i = 0; i < num_states; i++)
        optimized_states[i] = 0;

    // n * (n - 1) / 2
    // worst case: check all pairs of states that are not the same state
    int changes = 1;
    while (changes) {
        changes = 0;
        
        for (int i = 0; i < num_states && !optimized_states[i]; i++) {
            node_t equivalent_states = NULL;
            for (int j = i + 1; j < num_states && !optimized_states[j]; j++) {
                int are_equivalent_states = 1;
                for (int k = 0; k < alphabet_length + 1; k++) {
                    if (table[i][k] != table[j][k]) {
                        are_equivalent_states = 0;
                        break;
                    }
                }
                if (are_equivalent_states) {
                    if (equivalent_states == NULL)
                        equivalent_states__insert(&equivalent_states, i);
                    equivalent_states__insert(&equivalent_states, j);
                }
            }
            if (equivalent_states != NULL) {
                for (int ii = 0; ii < num_states; ii++) {
                    for (int jj = 0; jj < alphabet_length; jj++) {
                        if (equivalent_states__in(&equivalent_states,
                            table[ii][jj]))
                            table[ii][jj] =
                                equivalent_states__access_representative(
                                    &equivalent_states);
                    }
                }
                struct i_invalidate_optimized_states info =
                    { optimized_states };
                equivalent_states__access_non_representatives(U_QT,
                    &equivalent_states, &info, equivalent_states__predicate_1,
                        invalidate_optimized_states);
                // deallocate equivalent_states
                changes = 1;
            }
        }
    }

    // assert that the optimized FDA table is the same as the expected output
    assert(cmp_table((int **)output, (int **)table, optimized_states,
        num_states, alphabet_length));
}
