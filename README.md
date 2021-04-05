# libsapling

C library written for the *sapling*
[compiler](https://github.com/andriybyelikov/sapling) and
[editor](https://github.com/andriybyelikov/sapling-gui) suite.

**Disclaimer: the library is still in its initial development and the
code is ridden with memory leaks, I'm going for functionality first.**

---

## Implementations (by domain)

**```DATA MANAGEMENT [DM]```**

```
Path                Stack      Queue
Equivalence class   AVL tree   Trie
```

**```COMPILER CONSTRUCTION [CC]```**

```
Text         Terminal   Production
Parse Tree   Lexer      LR(0) Parser
```

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
#include <libsapling/data_management/stack.h>
```

Also remember to add ```-lsapling``` to your linker flags.

## Documentation

Doxygen documentation can be built.

---

## For mantainers

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

Fork it and send me a PR. If I like parts of your code I will merge them.
Otherwise just keep them to yourself.

I'm accepting refactored code with either less lines of code or less
information used or more modular code or new useful functionality. Check the
Tasks GitHub project of this repository.
