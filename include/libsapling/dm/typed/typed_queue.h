#ifndef LIBSAPLING_TYPED_QUEUE
#define LIBSAPLING_TYPED_QUEUE

/**
 * @file typed_queue.h
 * @brief Typed queue interface implementation
 */

#include "libsapling/dm/typed/typed_first_out_buffer.h"

#define IMPLEMENT_TYPED_QUEUE(SYM, TYPE, FPF) \
IMPLEMENT_TYPED_FIRST_OUT_BUFFER(SYM, TYPE, FPF, queue)

#endif
