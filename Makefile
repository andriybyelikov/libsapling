LIBDIR  := lib
OBJDIR  := obj
INCDIR  := include/libsapling
SRCDIR  := src
TESTDIR	:= test

CC      := gcc
CFLAGS  := -std=c11 -pedantic -I$(INCDIR) -g
LDFLAGS := -L$(LIBDIR) -lsapling

SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.c=.o)))
LIBRARY := $(LIBDIR)/libsapling.a
TESTS   := $(patsubst %.c, %.out, $(wildcard $(TESTDIR)/*.c))

.PHONY: all library tests clean run_tests install

all: library tests

library: $(LIBRARY)

$(LIBRARY): $(OBJECTS) | $(LIBDIR)
	ar rcs $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJDIR) $(LIBDIR):
	mkdir $@

tests: library $(TESTS)

$(TESTDIR)/%.out: $(TESTDIR)/%.c
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

clean:
	rm -rf $(OBJDIR) $(LIBDIR) $(TESTDIR)/*.out

run_tests: tests
	sh run_tests.sh

install:
	cp -a $(INCDIR) /usr/local/include
	cp $(LIBRARY) /usr/local/lib
