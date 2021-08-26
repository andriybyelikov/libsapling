#ifndef _LIBSAPLING_PARSER_AUX_PRODUCTION_SET_H_
#define _LIBSAPLING_PARSER_AUX_PRODUCTION_SET_H_

#include "libsapling/dm/path.h"
#include "libsapling/dm/queue.h"
#include "libsapling/cc/production.h"

IMPLEMENT_TYPED_PATH(production_path, production_t, production__print,
    dummy_cmp)
IMPLEMENT_TYPED_QUEUE(production_queue, production_t, production__print)

#endif
