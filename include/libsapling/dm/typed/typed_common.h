#ifndef _LIBSAPLING_TYPED_COMMON_H_
#define _LIBSAPLING_TYPED_COMMON_H_

/**
 * @file typed_common.h
 * @brief Useful common definitions for typed stuff
 */

#include <stdio.h>

/**
 * @brief File print format function for nodes holding integers
 */
static
void fpfdata_int(FILE *stream, const void *data)
{
    fprintf(stream, "%d", *(int *)data);
}

/**
 * @brief File print format function for nodes holding characters
 */
static
void fpfdata_char(FILE *stream, const void *data)
{
    fprintf(stream, "%c", *(char *)data);
}

/**
 * @brief File print format function for nodes holding strings
 */
static
void fpfdata_str(FILE *stream, const void *data)
{
    fprintf(stream, "%s", (const char *)data);
}

/**
 * @brief Dummy comparator function (if you don't want to specify one it when
 * using the IMPLEMENT_TYPED_X macro)
 */
static
int dummy_cmp(UNUSED const void *a, UNUSED const void *b)
{
    return -1;
}

#endif
