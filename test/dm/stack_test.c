#include <assert.h>
#include <string.h>
#include "libsapling/dm/stack.h"
#include "test/test_utils.h"

IMPLEMENT_TYPED_STACK(integer_stack, int, int__print)
DEFINE_OUTPUT_STATE_FUNC(integer_stack)

int main(int argc, char *argv[])
{
    TEST_PARSE_OPTIONS()

    // output empty stack state
    node_t stack = NULL;
    integer_stack__output_state(&stack);

    // push from 1 to 10 to the stack
    for (int i = 1; i <= 10; i++) {
        integer_stack__insert(&stack, i);
        integer_stack__output_state(&stack);
    }

    // assert length
    assert(integer_stack__length(&stack) == 10);

    // pop from 10 to 1 from the stack
    for (int i = 10; i >= 1; i--) {
        assert(integer_stack__delete(&stack) == i);
        integer_stack__output_state(&stack);
    }

    // assert length
    assert(integer_stack__length(&stack) == 0);

    // stack should now be empty
    assert(stack == NULL);
}
