#include <stdio.h>
#include "libsapling/idiom.h"

int int__compare(const void *a, const void *b)
{
    return *(int *)a <= *(int *)b;
}

int int__equals(const void *a, const void *b)
{
    return *(int *)a == *(int *)b;
}

void int__print(FILE *stream, const void *data)
{
    fprintf(stream, "%d", *(int *)data);
}


int char__compare(const void *a, const void *b)
{
    return *(char *)a <= *(char *)b;
}

int char__equals(const void *a, const void *b)
{
    return *(char *)a == *(char *)b;
}

void char__print(FILE *stream, const void *data)
{
    fprintf(stream, "%c", *(char *)data);
}


void string__print(FILE *stream, const void *data)
{
    fprintf(stream, "%s", *(const char **)data);
}

int dummy_cmp(UNUSED const void *a, UNUSED const void *b)
{
    return -1;
}
