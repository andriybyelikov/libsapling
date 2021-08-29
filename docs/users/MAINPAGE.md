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

@date 2021-08-29


## Examples

See the tests included with this library and you may also check out the
implementation of the sapling [compiler](https://github.com/andriybyelikov/sapling).

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
- @ref terminal.h "Terminal"
- @ref production.h "Production"
- @ref parse_tree.h "Parse Tree"
- @ref lexer.h "Lexer"
- @ref grammar.h "Grammar"
- @ref first.h "First"
- @ref follow.h "Follow"
- @ref lr0_item.h "LR(0) item"
- @ref closure.h "Closure"
- @ref goto.h "Goto"
- @ref action_table.h "Action table"
- @ref goto_table.h "Goto table"
- @ref slr.h "Simple LR"
