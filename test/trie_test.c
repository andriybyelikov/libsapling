#include <assert.h>
#include <string.h>
#include "graph.h"
#include "trie.h"

struct info_insert {
    size_t size;
    int index;
};

static
void fpd_int(FILE *fd, void **ref)
{
    int val = *(int *)trie__node__data(ref);
    fprintf(fd, "%d", val);
}

struct index {
    int index;
};

static
void get_idx(void **ref, void *info)
{
    if (*ref != NULL) {
        struct index *i = get_user_info(info);
        void *node = *ref;
        i->index = *(int *)trie__node__data(ref);
    }
}

int main(int argc, char *argv[])
{
    int dump_dot = argc > 1 && !strcmp(argv[1], "-v");

    void *trie = NULL;

    const char *words[] = {
        "lorem", "ipsum", "dolor", "sit", "amet", "consectetur", "adipiscing",
        "elit", "sed", "mollis", "wealth", "beyond", "measure", "outlander"
    };
    const size_t num_words = sizeof(words) / sizeof(const char *);

    struct info_insert ii = { .size = sizeof(int) };
    for (int i = 0; i < num_words; i++) {
        ii.index = i;
        trie__insert(&trie, words[i], &ii);
        if (dump_dot)
            trie__dump_dot(stdout, &trie, fpd_int);
    }
    for (int i = 0; i < num_words; i++) {
        struct index idx;
        trie__access(E_QT, &trie, words[i], &idx, get_idx);
        assert(i == idx.index);
    }
    for (int i = 0; i < num_words; i++) {
        trie__delete(&trie, words[i]);
        if (dump_dot)
            trie__dump_dot(stdout, &trie, fpd_int);
    }
    assert(trie == NULL);
}
