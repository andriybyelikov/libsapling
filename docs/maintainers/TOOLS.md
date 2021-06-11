# Tools

These are the tools that I use for development. - Andriy Byelikov
- gcc - C compiler
- bash - to run scripts
- Code - OSS (aka but not quite Visual Studio Code) - editor
- GNU Autotools - build and test system
- Valgrind - quickly check stack trace of a crash
- KDbg - GUI debugger with memory viewer

Scripts and programs in the ```tools``` directory
- checkstyle - checks the style in source files
- locreport - prints each tagged release's lines of code
- macroslash - adds backslashes to every line at the 79th column, useful when
  writing code generation macros
- Makefile.am.meta.sh - automatically generates lists of files int Makefile.am
- source_expansion.sh - deprecated, generates formatted code with macros
  expanded, altough the formatting doesn't work as I want
- vis.sh - you can feed this script output from a test with the -g flag and it
  will generate a folder with all the states of the graph rendered
