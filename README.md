# libsapling

C library written for the sapling [compiler](https://github.com/andriybyelikov/sapling)
and [editor](https://github.com/andriybyelikov/sapling-gui) suite.

---


**DISCLAIMER**

This library is in early development and is NOT non-functionally fit for
production. In particular, deallocation of dynamically allocated memory is not
implemented.

---


## Set of implementations by domain

### Data Management (DM)

Path ·
Stack ·
Queue ·
Equivalence class ·
AVL tree ·
Trie

### Compiler Construction (CC)

Text ·
Terminal ·
Production ·
Parse Tree ·
Lexer

---


## For users


### Building from source and installing

Using GNU Autotools.

```
$ mkdir build
$ cd build
$ ../configure
$ make
$ sudo make install
```

You can then use them in your code like this.

```
#include <libsapling/dm/stack.h>
```

Also remember to add ```-lsapling``` to your linker flags.

## Documentation

Doxygen documentation can be built.

---


## For maintainers

Build without optimization and run the tests like this.

```
$ mkdir build
$ cd build
$ ../configure CFLAGS='-O0 -g'
$ make check
```


### Implementation documentation

I have in mind writing special documentation for mantainers.


### Coding style

Read ```CODING_STYLE.md```.

### Tests and graph visualization

```make check```

To visualize the graph structure changes of a particular test do something like
this (requires Graphviz Dot installed).

```$ test/avl_test -g | sh vis.sh```

This will create a ```viz``` folder with the state images chronologically
enumerated.

TODO: from the build folder

```
make check
test/lexer_test -g | sh ../tools/viz
sh ../tools/gen_lexer_test_html > lexer_test.html
```

to generate an HTML file with all the generated automata from the lexer test
rendered


### Functional design flaws

predicateoverexposure solved

existentialnullcheck solved

struct info_insert indirect access to user information unsolved

### Contributing

Before contributing please consider that for the future of this project I plan
to write and use a custom LGPL-like license that is significantly stricter than
the current zlib/libpng license I am currently using, with the necessary
restrictions and exceptions to both protect the future of the project, by
ensuring that improvements are shared back, and proprietary code that links
with it from having to be shared.

I am mainly thinking in the context of commercial video games, which may
benefit from porting but hiding content logic

if not i will have to throw out your code
