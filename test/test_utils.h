#ifndef LIBSAPLING_TEST_UTILS
#define LIBSAPLING_TEST_UTILS

#include <stdio.h>
#include "libsapling/dm/graph.h"


#define PARSE_OPTIONS()                                                       \
printf("%s\n", argv[0]);                                                      \
for (int i = 1; i < argc; i++)                                                \
    if (*argv[i] == '-')                                                      \
        switch (*(argv[i] + 1)) {                                             \
        case 'p':                                                             \
            flag_print_data = 1;                                              \
            break;                                                            \
        case 'g':                                                             \
            flag_dump_dot = 1;                                                \
            break;                                                            \
        }

#define DEFINE_OUTPUT_STATE_FUNC(SYM)                                         \
static int flag_print_data = 0;                                               \
static int flag_dump_dot = 0;                                                 \
static                                                                        \
void SYM##__output_state(node_t *ref)                                         \
{                                                                             \
    if (flag_print_data)                                                      \
        SYM##__print_data(stdout, ref);                                       \
    else if (flag_dump_dot)                                                   \
        SYM##__dump_dot(stdout, ref);                                         \
}

#endif
