#include <assert.h>
#include "libsapling/dm/trie.h"
#include "test_utils.h"

static
void print_integer(FILE *stream, const void *data)
{
    // data is pointer to data, need double dereference
    if (*(void **)data != NULL)
        fprintf(stream, "%d", **(int **)data);
}

IMPLEMENT_TYPED_TRIE(it, int, print_integer)


struct index_key {
    int index;
    int found;
};

static
int find_key_by_index(const int *data, void *info)
{
    CAST_USER_INFO(struct index_key *, user, info);

    return **(int **)data == user->index;
}

static
void found_key(UNUSED int *data, void *info)
{
    CAST_USER_INFO(struct index_key *, user, info);

    user->found = 1;
}


DEFINE_OUTPUT_STATE_FUNC(it)

int main(int argc, char *argv[])
{
    TEST_PARSE_OPTIONS()

    const char *keys[] = { "wealth", "beyond", "measure", "outlander", "dio",
        "diocles", "diocletianus", "flavius", "flavian", "florianus" };
    const int num_keys = sizeof(keys) / sizeof(const char *);

    // output empty trie state
    node_t trie = NULL;
    it__output_state(&trie);

    // push (keys[i], i) to the trie
    for (int i = 0; i < num_keys; i++) {
        it__insert(&trie, keys[i], i);
        it__output_state(&trie);
    }

    // assert length
    assert(it__length(&trie) == num_keys);

    // assert that key with value i exists and is keys[i]
    for (int i = 0; i < num_keys; i++) {
        struct index_key info = { i, 0 };
        it__access(E_QT, &trie, keys[i], &info, find_key_by_index, found_key);
        assert(info.found);
    }

    // delete keys
    for (int i = 0; i < num_keys; i++) {
        it__delete(&trie, keys[i]);
        it__output_state(&trie);
    }

    // assert length
    assert(it__length(&trie) == 0);

    // trie should now be empty
    assert(trie == NULL);
}
