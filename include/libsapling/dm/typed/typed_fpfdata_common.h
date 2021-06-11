#ifndef _LIBSAPLING_TYPED_FPFDATA_COMMON_H_
#define _LIBSAPLING_TYPED_FPFDATA_COMMON_H_

#include <stdio.h>

void fpfdata_int(FILE *stream, const void *data)
{
    fprintf(stream, "%d", *(int *)data);
}

#endif
