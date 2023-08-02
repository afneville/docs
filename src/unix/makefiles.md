---
title: Writing Makefiles
date: 2023-07-24
---

For many small to medium size C projects, it is very convenient to split
source files across three sub-directories, according to their purpose.
`./inc/` is for header files, `./src/` is for generic source files, and
`./run/` is for source files for which an executable should be
generated.

```text
./
├──bin/
│  └──test
├──inc/
│  ├──item.h
│  └──list.h
├──obj/
│  ├──item.o
│  └──list.o
├──run/
│  └──test.c
├── src/
│  ├──item.c
│  ├──list.c
│  └──tree.c
├──.gitignore
├──compile_commands.json
├──makefile
└──README.org
```

The project root directory contains the makefile.
`compile_commands.json` exists for language server
configuration, GCC is used in the makefile. The clean target of make
will fully remove the binary and object file directories. Fully
rebuilding the project in a single command:

```make
MKDIR   := mkdir
RMDIR   := rm -rf
CC      := gcc
BIN     := ./bin
OBJ     := ./obj
INCLUDE := ./inc
SRC     := ./src
APP		:= ./run
SRCS    := $(wildcard $(SRC)/*.c)
APPS    := $(wildcard $(APP)/*.c)
TESTS   := $(wildcard ./tests/*.c)
OBJS    := $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))
PROGS   := $(patsubst $(APP)/%.c,$(BIN)/%,$(APPS))
CFLAGS  := -I $(INCLUDE)
LDLIBS  :=

.PHONY: all run clean build

build: $(PROGS)

$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN)/%: $(APP)/%.c $(OBJS) | $(BIN)
	$(CC) $^ -o $@ $(LDLIBS)

$(BIN):
	$(MKDIR) $@

$(OBJ):
	$(MKDIR) $@

clean:
	$(RMDIR) $(OBJ) $(BIN)

lspconfig:
	bear -- make 2> /dev/null
```
