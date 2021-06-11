#ifndef LIBSAPLING_TYPED_FIRST_OUT_BUFFER
#define LIBSAPLING_TYPED_FIRST_OUT_BUFFER

/**
 * @file typed_first_out_buffer.h
 * @brief Metaimplementation of the typed stack and queue interfaces
 */

#define IMPLEMENT_TYPED_FIRST_OUT_BUFFER(SYM, TYPE, FPF, IMPL)                \
                                                                              \
static                                                                        \
void SYM##__insert(node_t *ref, TYPE value)                                   \
{                                                                             \
    struct info_insert info = { sizeof(TYPE), &value };                       \
    IMPL##__insert(ref, &info);                                               \
}                                                                             \
                                                                              \
static                                                                        \
TYPE SYM##__access(node_t *ref)                                               \
{                                                                             \
    return *(TYPE *)IMPL##__access(ref);                                      \
}                                                                             \
                                                                              \
static                                                                        \
TYPE SYM##__delete(node_t *ref)                                               \
{                                                                             \
    TYPE retval = *(TYPE *)IMPL##__access(ref);                               \
    IMPL##__delete(ref);                                                      \
    return retval;                                                            \
}                                                                             \
                                                                              \
static                                                                        \
void SYM##__print_data(FILE *stream, node_t *ref)                             \
{                                                                             \
    IMPL##__print_data(stream, ref, FPF);                                     \
}                                                                             \
                                                                              \
static                                                                        \
void SYM##__dump_dot(FILE *stream, node_t *ref)                               \
{                                                                             \
    IMPL##__dump_dot(stream, ref, FPF);                                       \
}

#endif
