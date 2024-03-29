#include <stdlib.h>
#include "libsapling/idiom.h"
#include "libsapling/dm/path.h"
#include "libsapling/cc/production.h"

IMPLEMENT_TYPED_PATH(body, int, NULL, dummy_cmp)

struct production {
    int id;
    node_t body;
};


struct path_length {
    int len;
};

static
void path_length(UNUSED const int *data, void *info)
{
    struct path_length *user = info;

    user->len++;
}


production_t new_production(int id, node_t *body)
{
    production_t p = malloc(sizeof(struct production));
    p->id = id;
    p->body = *body;
    return p;
}


int production__id(production_t ref)
{
    return ref->id;
}

int production__len(production_t ref)
{
    return body__length(&ref->body);
}

int *production__get(production_t ref, int pos)
{
    return body__position(&ref->body, pos);
}

void production__print(FILE *stream, const void *data)
{
    production_t prod = *(production_t *)data;

    fprintf(stream, "[%d -> ", production__id(prod));
    int i;
    for (i = 0; i < production__len(prod) - 1; i++) {
        fprintf(stream, "%d ", *production__get(prod, i));
    }
    fprintf(stream, "%d]", *production__get(prod, i));
}
