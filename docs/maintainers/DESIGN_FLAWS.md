# Design flaws

### [SOLVED] Predicate and apply function overexposure

`predicate_t` and `apply_t` are defined as:

`typedef int (*predicate_t)(const node_t *ref, const struct info_stack *info);`

`typedef void (*apply_t)(node_t *ref, const struct info_stack *info);`

which exposes too much information to the user who when writing a predicate or
an apply function had to call the `X_data` function to extract and then cast
the data held by the node to the appropiate node type and also had to access
the user info from the `info_stack` struct.

With the creation of typed interfaces (see `typed_adapters.h`), predicates and
apply functions can be defined with direct access to the typed pointer to the
data and the user information pointer.

`int typed_predicate(const TYPE *data, void *info)`

`void typed_apply(TYPE *data, void *info)`

### [SOLVED] Existential NULL check

In `graph.c` if you look at the `e_loop` function you will notice that unlike
`u_loop`, `e_loop` doesn't NULL check whether the referred node is NULL before
calling the apply function. This is correct and its purpose is to allow for
insertion. It means that a new node must be appended at that reference. This
however will throw a SEGV, so users always had to check for a reference that
pointed to NULL when writing an apply function. This check is no longer
required as it was absorbed by the typed adapters to the predicate and apply
functions, so typed apply functions don't need to check for references pointing
to NULL.

### [UGLY SOLUTION] Indirect access to user information

`struct info_insert` is used by the insert functions to specify the data to be
copied to the new node's data storage and its size, and it is also used by the
`in` functions of the typed interfaces to store the key object and can be used
by some typed interfaces to delete or access objects with the `equ_preicate`.
This means that the user information isn't passed directly but through a member
pointer of `struct info_insert`, requiring one extra dereference by users.
Currently, the `CAST_USER_INFO` macro in `typed_adapters.h` can be used as a
short way of doing this but it is not very elegant.
