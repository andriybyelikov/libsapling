#include <assert.h>
#include <string.h>
#include "test/test_utils.h"
#include "libsapling/dm/queue.h"

static
void fpf_int(FILE *stream, const void *data)
{
    fprintf(stream, "%d", *(int *)data);
}

IMPLEMENT_TYPED_QUEUE(integer_queue, int, fpf_int)
DEFINE_OUTPUT_STATE_FUNC(integer_queue)


int main(int argc, char *argv[])
{
    PARSE_OPTIONS()

    // Output empty queue state
    node_t queue = NULL;
    integer_queue__output_state(&queue);

    // Push from 1 to 10 to the queue
    for (int i = 1; i <= 10; i++) {
        integer_queue__insert(&queue, i);
        integer_queue__output_state(&queue);
    }

    // Pop from 1 to 10 from the queue
    for (int i = 1; i <= 10; i++) {
        assert(integer_queue__delete(&queue) == i);
        integer_queue__output_state(&queue);
    }

    // Queue should now be empty
    assert(queue == NULL);
}
