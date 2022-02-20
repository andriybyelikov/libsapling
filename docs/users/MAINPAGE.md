# Documentation for users

Click [here](../../maintainers/html/index.html) if you were looking for the
documentation for maintainers.

C library written for the sapling
[compiler](https://github.com/andriybyelikov/sapling) and
[editor](https://github.com/andriybyelikov/sapling-gui) suite.

The library consists of a set of data structures, utilities and building blocks
written with orthogonality in mind while also aiming to express their
implementation with the fewest lines of code possible and in terms of each
other.

@author Andriy Byelikov

@date 2022-01-17


## Examples

See the tests included with this library and you may also check out the
implementation of the
sapling [compiler](https://github.com/andriybyelikov/sapling).

## Domains

- @ref idiom.h "Idiom"

### Data Management

Please see @ref TYPED_INTERFACES.md.

- @ref logic.h "Logic"
- @ref graph.h "Graph"
- @ref path.h "Path"
- @ref stack.h "Stack"
- @ref queue.h "Queue"
- @ref equivalence_class.h "Equivalence class"
- @ref avl.h "AVL"
- @ref trie.h "Trie"

### Compiler Construction

- @ref text.h "Text"
- @ref lexer.h "Lexer"
- @ref parse_tree.h "Parse Tree"
- @ref terminal.h "Terminal"
- @ref production.h "Production"
- @ref grammar.h "Grammar"
- @ref slr.h "Simple LR Parser"
