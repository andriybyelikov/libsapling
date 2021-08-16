# libsapling

C library written for the sapling
[compiler](https://github.com/andriybyelikov/sapling) and
[editor](https://github.com/andriybyelikov/sapling-gui) suite.

The library consists of a set of data structures, utilities and building blocks
written with orthogonality in mind while also aiming to express their implementation with the fewest lines of code possible and in terms of each
other.

As an example of the latter, we start with the implementation of a graph with
138 lines, which helps implement a path with 118 lines, which helps implement a
first-out buffer with 38 lines, and from the first-out buffer, both the stack
and the queue data structures are implemented with just 1 line each.

```
graph 138
\_ path 118
   \_ first-out buffer 38
      \_ stack 1
      \_ queue 1
```

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
AVL ·
Trie

### Compiler Construction (CC)

Text ·
Terminal ·
Production ·
Parse Tree ·
Lexer

---


## For users

[Link to documentation](https://andriybyelikov.github.io/libsapling/docs/users/html/index.html)

### Building from source and installing

Using GNU Autotools.

```
$ autoreconf --install
$ mkdir build
$ cd build
$ ../configure
$ make
$ sudo make install
```

You can then use it in your code like this:

```
#include <libsapling/dm/stack.h>
```

Also remember to add ```-lsapling``` to your linker flags.

### Examples

See the tests included with this library and you may also check out the
implementation of the sapling [compiler](https://github.com/andriybyelikov/sapling).

---

## For maintainers

[Link to documentation](https://andriybyelikov.github.io/libsapling/docs/maintainers/html/index.html)

Build without optimization and run the tests like this:

```
$ autoreconf --install
$ mkdir build
$ cd build
$ ../configure CFLAGS='-O0 -g'
$ make check
```

### Development

`tools/Makefile.am.meta` is a shell script that is used to automatically detect
and add source files to `Makefile.am`.

### Tests and graph visualization

This assumes that your are in the build directory.

First build the tests:

`$ make check`

Almost all tests have two options `-p` or print data and `-g` dump Dot graph.
The only test that has none of these options is the `equivalence_class` test,
and the `parse_tree_test` and `lexer_test` have no `-p` option.

To visualize the evolution of the graph structures of a particular test do
something like this (requires Graphviz Dot installed).

`$ test/avl_test -g | sh ../tools/viz`

This will create a `viz` folder with the `.svg` images of the states of the 
graph structures chronologically enumerated.

You can also run `$ sh ../tools/viz_all_tests` to generate the snapshots for
all the tests that support it. This will take a few seconds depending on your
machine.

In the case of the `lexer_test` you can generate an HTML page to more
comfortably visualize and compare the resulting automata.

```
$ test/lexer_test -g | sh ../tools/viz
$ sh ../tools/gen_lexer_test_html > lexer_test.html
```

---

## Arch Linux packages

[Link to repository with PKGBUILD scripts](https://github.com/andriybyelikov/archlinux-sapling-packages)

---

## Contributing

Before contributing please consider that for the future of this project I plan
to write and use a custom LGPL-like license that is significantly stricter than
the current zlib/libpng license I am currently using, with the necessary
restrictions and exceptions to both protect the future of the project, by
ensuring that improvements are shared back, and proprietary code that links
with it from having to be shared.

TL;DR I have no idea what kind of license will better suit the future of the
project so it's zlib/libpng for now. I know that I want to use it in my own
commercial videogames without having to expose the more creative parts of the
source code (e.g. content logic).
