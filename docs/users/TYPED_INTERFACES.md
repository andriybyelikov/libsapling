# Typed interfaces

Typed interfaces are much like generics in Java, but look more like generics in
Ada in the sense that you have to instantiate them:

`IMPLEMENT_TYPED_X(SYM, TYPE, CMP, EQU, FPF)`

Where X is one of the following:
- `PATH`
- `STACK`
- `QUEUE`
- `AVL`
- `TRIE`
- `EQUIVALENCE_CLASS`

All interfaces have at least the `SYM`, `TYPE` and `FPF` parameters, while the
`CMP` and `EQU` parameters may both appear, or just one of them or none at all.

@param SYM Name of the instance. This serves as the prefix for the method
definitions. For example if you called the macro
`IMPLEMENT_TYPED_STACK(stack_of_integers, int, fpfdata_int)` then to use this
instance's methods you would write `stack_of_integers__insert(&stack, 21);`.
Naturally, `SYM` can't be of the base implementations' names, at least not in
their all lowercase form, unless you don't include them in your source file:
- `graph`
- `path`
- `stack`
- `queue`
- `avl`
- `trie`
- `equivalence_class`
- `parse_tree`
- `lexer`

@param TYPE The type the instance will be dealing with.

@param FPF File print format function for the data the nodes hold. There are
some predefined ones in @ref typed_common.h

@param CMP Comparator function (e.g. function that returns `a < b`)
@param EQU Equality function (e.g. function that returns `a == b`). Can be
`dummy_cmp` if you don't want to specify one, but then you won't have a working
`in` function.

## The in function

Some typed interfaces (path, AVL and equivalence class) define a working `in`
function when a correct `EQU` function is specified during instantiation
(e.g. not `dummy_cmp`). This function takes an object of the instance type and
tells whether there is an equal object in the set according to the specified
`EQU` function.
