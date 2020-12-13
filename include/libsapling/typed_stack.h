#include <libsapling/stack.h>

#define define_typed_stack(type)                  \
                                                  \
struct insert_##type {                            \
    size_t size;                                  \
    type data;                                    \
};                                                \
                                                  \
static                                            \
void stack_##type##__insert(void **ref, type val) \
{                                                 \
    struct insert_##type info = {                 \
        sizeof(type),                             \
        val                                       \
    };                                            \
    stack__insert(ref, &info);                    \
}                                                 \
                                                  \
static                                            \
void stack_##type##__delete(void **ref)           \
{                                                 \
    stack__delete(ref);                           \
}                                                 \
                                                  \
static                                            \
type stack_##type##__access(void **ref)           \
{                                                 \
    return *(type *)stack__access(ref);           \
}
