#ifndef _LIBSAPLING_TYPED_COMMON_H_
#define _LIBSAPLING_TYPED_COMMON_H_

/**
 * @file typed_common.h
 * @brief Useful common definitions for typed stuff
 */

#include <stdio.h>
#include "libsapling/idiom.h"

/**
 * @brief File print format function for nodes holding integers
 */
void fpfdata_int(FILE *stream, const void *data);

/**
 * @brief File print format function for nodes holding characters
 */
void fpfdata_char(FILE *stream, const void *data);

/**
 * @brief File print format function for nodes holding strings
 */
void fpfdata_str(FILE *stream, const void *data);

/**
 * @brief Dummy comparator function (if you don't want to specify one it when
 * using the IMPLEMENT_TYPED_X macro)
 */
int dummy_cmp(UNUSED const void *a, UNUSED const void *b);

#endif
