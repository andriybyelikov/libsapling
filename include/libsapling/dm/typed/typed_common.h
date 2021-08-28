#ifndef _LIBSAPLING_TYPED_COMMON_H_
#define _LIBSAPLING_TYPED_COMMON_H_

/**
 * @file typed_common.h
 * @brief Useful common definitions for typed stuff
 */

#include <stdio.h>
#include "libsapling/dm/graph.h"


typedef node_t *pnode_t;

int int__compare(const void *a, const void *b);
int int__equals(const void *a, const void *b);
void int__print(FILE *stream, const void *data);

int char__compare(const void *a, const void *b);
int char__equals(const void *a, const void *b);
void char__print(FILE *stream, const void *data);

void string__print(FILE *stream, const void *data);

/**
 * @brief Dummy comparator function (if you don't want to specify one it when
 * using the IMPLEMENT_TYPED_X macro)
 */
int dummy_cmp(UNUSED const void *a, UNUSED const void *b);

#endif
