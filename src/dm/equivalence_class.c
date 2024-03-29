#include "libsapling/dm/path.h"
#include "libsapling/dm/queue.h"

void *equivalence_class__data(node_t ref)
{
    return path__data(ref);
}

void equivalence_class__insert(node_t *ref, void *info)
{
    queue__insert(ref, info);
}

void *equivalence_class__access_representative(node_t *ref)
{
    return queue__access(ref);
}

void equivalence_class__access_non_representatives(enum qt qt, node_t *ref,
    void *info, predicate_t predicate, apply_t apply)
{
    path__access(qt, path__position(ref, 1), info, predicate, apply);
}
