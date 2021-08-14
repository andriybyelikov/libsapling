#include <assert.h>
#include <string.h>
#include "libsapling/dm/queue.h"
#include "libsapling/dm/typed/typed_common.h"
#include "test/test_utils.h"

IMPLEMENT_TYPED_QUEUE(integer_queue, int, fpfdata_int)
DEFINE_OUTPUT_STATE_FUNC(integer_queue)

int main(int argc, char *argv[])
{
    TEST_PARSE_OPTIONS()

    // output empty queue state
    node_t queue = NULL;
    integer_queue__output_state(&queue);

    // push from 1 to 10 to the queue
    for (int i = 1; i <= 10; i++) {
        integer_queue__insert(&queue, i);
        integer_queue__output_state(&queue);
    }

    // assert length
    assert(integer_queue__length(&queue) == 10);

    // pop from 1 to 10 from the queue
    for (int i = 1; i <= 10; i++) {
        assert(integer_queue__delete(&queue) == i);
        integer_queue__output_state(&queue);
    }

    // assert length
    assert(integer_queue__length(&queue) == 0);

    // queue should now be empty
    assert(queue == NULL);
}
