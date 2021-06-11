# Coding style

## C

K&R style.


### Headers

0. Prefix header guard symbols with ```LIBSAPLING_```.


### Functions

- The ```static``` modifier should be on a line by itself above the function
  signature.
- Put parameter casts at the beginning of the function and leave an empty line
  between them and the rest of the code.
- Do not prepend the ```const``` modifier to scalar parameters that are passed
  by value if it is not necessary to do so.
- When similar functions only differ in some extra parameters put those as the
  last parameters, keeping the common parameters first and in the same order.
  - One exception to this rule is the quantifier parameter in data management
    implementations, which should always be the first parameter, but some
    applications only allow one of them.


### Source files

- Sort constructs by:
  - Dependency (i.e. if a construct depends on another, the latter should be
    above it)
  - If the construct does not depend on other constructs in the same file then
    use this order:
    - (1) Constructs that you feel may be implemented outside the
      file in the future
    - (2) Construct whose implementation is trivial/simpler/shorter
- Group together constructs that have a linear dependency path, separating them
  from the rest of constructs by wrapping them in two empty lines befor and
  after (i.e. a function that uses pseudo-lambdas that are only used by it
  would be a group started by two empty lines, a struct, empty line, a
  predicate function, empty line, an apply function, empty line, the function
  itself, then finally two empty lines).
