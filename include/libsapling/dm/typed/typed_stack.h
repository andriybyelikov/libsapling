#ifndef LIBSAPLING_TYPED_STACK
#define LIBSAPLING_TYPED_STACK

/**
 * @file typed_stack.h
 * @brief Typed stack interface implementation
 */

#include "libsapling/dm/typed/typed_first_out_buffer.h"

#define IMPLEMENT_TYPED_STACK(SYM, TYPE, FPF) \
IMPLEMENT_TYPED_FIRST_OUT_BUFFER(SYM, TYPE, FPF, stack)

#endif
