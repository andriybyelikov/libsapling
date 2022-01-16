#ifndef _LIBSAPLING_TEST_UTILS_H_
#define _LIBSAPLING_TEST_UTILS_H_

#include <stdio.h>
#include "libsapling/dm/graph.h"

#define TEST_PARSE_OPTIONS()                                                  \
printf("%s\n", argv[0]);                                                      \
for (int i = 1; i < argc; i++) {                                              \
    if (*argv[i] == '-') {                                                    \
        switch (*(argv[i] + 1)) {                                             \
        case 'p':                                                             \
            flag_print = 1;                                                   \
            break;                                                            \
        case 'g':                                                             \
            flag_dump_dot = 1;                                                \
            break;                                                            \
        }                                                                     \
    }                                                                         \
}

#define DEFINE_OUTPUT_STATE_FUNC(SYM)                                         \
static int flag_print = 0;                                                    \
static int flag_dump_dot = 0;                                                 \
static                                                                        \
void SYM##__output_state(node_t *ref)                                         \
{                                                                             \
    if (flag_print) {                                                         \
        SYM##__println(stdout, ref);                                          \
    } else if (flag_dump_dot) {                                               \
        printf(#SYM "\n");                                                    \
        SYM##__dump_dot(stdout, ref);                                         \
    }                                                                         \
}

#endif
