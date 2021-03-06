#include <stddef.h>
#include "graph.h"
#include "path.h"

size_t stack__edge_storage(void)
{
    return path__edge_storage();
}

void *stack__node__data(void **ref)
{
    return path__node__data(ref);
}


void stack__insert(void **ref, void *info)
{
    path__insert(ref, info, match_1);
}

void stack__delete(void **ref)
{
    path__delete(E_QT, ref, NULL, match_1);
}


static
void get_data(void **ref, void *info)
{
    if (*ref != NULL)
        **(void ***)info = stack__node__data(ref);
}

void *stack__access(void **ref)
{
    void *data;
    path__access(E_QT, ref, &data, match_1, get_data);
    return data;
}


void stack__dump_dot(FILE *fd, void **ref, void (*fpd)(FILE *fd, void **ref))
{
    path__dump_dot(fd, ref, fpd);
}
