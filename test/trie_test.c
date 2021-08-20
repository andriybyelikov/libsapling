#include <assert.h>
#include "libsapling/dm/trie.h"
#include "libsapling/dm/typed/typed_common.h"
#include "test/test_utils.h"

IMPLEMENT_TYPED_TRIE(it, int, fpfdata_int)


struct index_key {
    int index;
    int found;
};

static
int key_has_value(const int *data, void *info)
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
        it__access(E_QT, &trie, keys[i], &info, key_has_value, found_key);
        assert(info.found);
    }

    // try to access a key that is not in the set
    assert(!it__in(&trie, "Sheogorath"));

    // try to access a key that is in the set
    assert(it__in(&trie, "outlander"));

    // make sure that invalid prefixes that are part of a valid key, that is,
    // part of a key whose nodes exist but the node corresponding to the last
    // character of the key is not an accepting state, are not considered as
    // being in the set
    assert(!it__in(&trie, "out"));

    // delete keys
    for (int i = 0; i < num_keys; i++) {
        it__delete(&trie, keys[i]);
        it__output_state(&trie);
    }

    // assert length
    assert(it__length(&trie) == 0);

    // trie should now be empty
    assert(trie == NULL);

    // make sure that the in function returns 0 when the trie is empty
    assert(it__in(&trie, "whatever") == 0);
}
