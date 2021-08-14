#ifndef _LIBSAPLING_TYPED_STACK_H_
#define _LIBSAPLING_TYPED_STACK_H_

/**
 * @file typed_stack.h
 * @brief Typed stack interface implementation
 */

#include "libsapling/dm/typed/typed_first_out_buffer.h"

/**
 * @brief Defines a typed stack implementation
 * 
 * @param SYM Prefix symbol of all implemented functions
 * @param TYPE Type with which the implementation will be dealing with
 * @param FPF fpfdata function that prints the formatted data of the
 * implementation's type
 */
#define IMPLEMENT_TYPED_STACK(SYM, TYPE, FPF) \
IMPLEMENT_TYPED_FIRST_OUT_BUFFER(SYM, TYPE, FPF, stack)

#endif
