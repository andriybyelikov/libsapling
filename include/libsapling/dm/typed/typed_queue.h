#ifndef _LIBSAPLING_TYPED_QUEUE_H_
#define _LIBSAPLING_TYPED_QUEUE_H_

/**
 * @file typed_queue.h
 * @brief Typed queue interface implementation
 */

#include "libsapling/dm/typed/typed_first_out_buffer.h"

/**
 * @brief Defines a typed queue implementation
 * 
 * @param SYM Prefix symbol of all implemented functions
 * @param TYPE Type with which the implementation will be dealing with
 * @param FPF fpfdata function that prints the formatted data of the
 * implementation's type
 */
#define IMPLEMENT_TYPED_QUEUE(SYM, TYPE, FPF) \
IMPLEMENT_TYPED_FIRST_OUT_BUFFER(SYM, TYPE, FPF, queue)

#endif
