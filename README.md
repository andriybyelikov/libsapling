# libsapling

Library for the sapling compiler and editor suite.

Currently it's mostly """generic""" graph based collection implementations.

## Available collection implementations

- Path
- Stack
- Queue
- AVL Tree
- Trie

## General Usage

First, allocate what will be an edge (pointer) to a starting node.

**It is important to initialize the edge to ```NULL```.**

```
void *edge = NULL;
```

After this you can start applying operations passing the edge by reference
```&edge```. The node this edge points to will be automatically updated after a
successful insertion/deletion operation or set to ```NULL``` when the
collection becomes empty.

### Operations

There are three universal operations: insert, delete and access.

These can be classified in two ways.

Operations that can mutate the structure of the collection:
- insert: *iᵢ* ∉ *S* ⇒ *iᵢ* ∈ *S*
- delete: *iᵢ* ∈ *S* ⇒ *iᵢ* ∉ *S*

Operations that can select items in the collection:
- delete: *S'* = { *iᵢ* | *iᵢ* ∈ *S* ∧ m(*i*) }, ∀*i* ∈ *S'* ⇒ ∀*i* ∉ *S'*
- access: *S'* = { *iᵢ* | *iᵢ* ∈ *S* ∧ m(*i*) }, ∀*i* ∈ *S'* ⇒ a(*i*)

Many operations take helper decision functions as parameters and a
```void *info``` pointer to external user defined information that can be used
in the helper decision function.

A notable case is insertion where the first field must be a ```size_t``` that
specifies the size of the data to be stored in the node followed by the data
itself and this may be followed by extra user defined information.

Also be warned that deleting a node will also deallocate the memory previously
allocated for its data so make sure to keep track of dangling pointers.


## Quantifiers

Some implementations may accept a quantifier as a parameter which invokes
different ways of traversing the collection's graph to get to the data.

Universal Quantifier ∀ ```U_QT``` (Equivalent to traversal)
- This one visits all the nodes.

Existential Quantifier ∃ ```E_QT``` (Equivalent to search)
- This one tries to get to the target node as fast as possible.

Here is a list of all implemented collections and their implemented
quantifiers.

Structure|insert|delete|access
:-:|:-:|:-:|:-:
path|∃|∀∃|∀∃
stack|∃|∃|∀*∃
queue|∃|∃|∀*∃
avl|∃|∃|∀**∃
trie|∃|∀∃|∀**∃

\* For complete deallocation of a path based collection you can invoke path
deletion targeting all nodes.

\** Tree based collection traversal here is done in no useful order, just
traversing all to get a Graphviz Dot dump.

As can be seen here, all insertion implementations are done as a search of the
edge where the node must be placed. I do not know if there is any useful
meaning in a traversal insertion. Stack and queue insertion and deletion are
trivial searches by their own nature.

## Documentation

No standalone documentation is provided as of right now. For now I direct you
to check out ```graph.h``` and the test examples.

## Tests and graph visualziation

To check that the tests are passed run the following command and make sure that
no failed assertion message pops up.

```make run_tests```

To visualize the graph structure changes of a particular test do something like
this (requires Graphviz Dot installed).

```./test/avl_test.out -v | sh to_dot.sh```

This will create a ```vis``` folder with the state images chronologically
enumerated.

## Implementation

The collections are implemented in the C11 revision of the C programming
language, with the pedantic flag set in GCC. Currently compiled with debugging
symbols and no optimizations.

#### Node Structure

Offset|Data
:-|:-
0|Edge Storage
0 + size of edge storage|Data Storage (May not exist)

## Contributing

I'm accepting refactored code with either less lines of code or less
information used or more modular code or new useful functionality. Check the
GitHub project Tasks.
