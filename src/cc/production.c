#include <stdlib.h>
#include "libsapling/idiom.h"
#include "libsapling/dm/path.h"
#include "libsapling/cc/production.h"

IMPLEMENT_TYPED_PATH(body, int, NULL)

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


void *production(int sid, int nsym, node_t *body)
{
    int *p = malloc((2 + nsym) * sizeof(int));
    p[0] = sid;
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
