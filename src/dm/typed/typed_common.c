#include <stdio.h>
#include "libsapling/idiom.h"

void fpfdata_int(FILE *stream, const void *data)
{
    fprintf(stream, "%d", *(int *)data);
}

void fpfdata_char(FILE *stream, const void *data)
{
    fprintf(stream, "%c", *(char *)data);
}

void fpfdata_str(FILE *stream, const void *data)
{
    fprintf(stream, "%s", *(const char **)data);
}

int dummy_cmp(UNUSED const void *a, UNUSED const void *b)
{
    return -1;
}
