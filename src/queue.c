#include <stddef.h>
#include "graph.h"
#include "path.h"

size_t queue__edge_storage(void)
{
    return path__edge_storage();
}

void *queue__node__data(void **ref)
{
    return path__node__data(ref);
}


void queue__insert(void **ref, void *info)
{
    path__insert(ref, info, match_0);
}

void queue__delete(void **ref)
{
    path__delete(E_QT, ref, NULL, match_1);
}

static
void get_data(void **ref, void *info)
{
    if (*ref != NULL)
        **(void ***)info = queue__node__data(ref);
}

void *queue__access(void **ref)
{
    void *data;
    path__access(E_QT, ref, &data, match_1, get_data);
    return data;
}

void queue__dump_dot(FILE *fd, void **ref, void (*fpd)(FILE *fd, void **ref))
{
    path__dump_dot(fd, ref, fpd);
}
