#include <string.h>
#include "libsapling/dm/queue.h"

IMPLEMENT_TYPED_QUEUE(str_queue, const char *, string__print)

int main(void)
{
    node_t str_queue = NULL;
    str_queue__insert(&str_queue, "wealth");
    str_queue__insert(&str_queue, "beyond");
    str_queue__insert(&str_queue, "measure");
    str_queue__insert(&str_queue, "outlander");
    str_queue__print_data(stdout, &str_queue);
    fprintf(stdout, "\n");
}
