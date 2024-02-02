---
title: Building a Project with CMake
author: Alexander Neville
date: 2024-01-28
description: A guide to using and understanding the CMake utility.
path: dev/unix/cmake
image-meta: cmake-logo.png
image-meta-credit: https://cmake.org/
---

CMake is a widely used build automation tool for C and C++ projects.
CMake is not (strictly speaking) a build system, however it is able to
generate configuration files for a number of different build systems,
making it a platform and compiler agnostic build tool. Rather than
simultaneously maintaining a makefile and a Visual Studio configuration
for compilation on Unix and Windows respectively, specifying build
instructions with CMake allows one to automatically generate build
configurations for any system.

# Example Makefile Project

Consider the small C project outlined in the tree below. It comprises a
small library consisting of two source files, two corresponding header
files and two source files for which an executable is to be generated.

```text
.
|-- inc
|   |-- item.h
|   `-- list.h
|-- run
|   |-- item_test.c
|   `-- list_test.c
`-- src
    |-- item.c
    `-- list.c
```

A [makefile](./makefiles.md) could be used to compile the library and
link it with each executable. Such a configuration might resemble this
example:

```makefile
CC      := gcc
BIN     := ./bin
OBJ     := ./obj
INCLUDE := ./inc
SRC     := ./src
APP     := ./run
SRCS    := $(wildcard $(SRC)/.c)
APPS    := $(wildcard $(APP)/.c)
TESTS   := $(wildcard ./tests/*.c)
OBJS    := $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))
PROGS   := $(patsubst $(APP)/%.c,$(BIN)/%,$(APPS))
CFLAGS  := -I $(INCLUDE)
LDLIBS  :=
.PHONY: build
build: $(PROGS)$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@
$(BIN)/%: $(APP)/%.c $(OBJS) | $(BIN)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)
$(BIN):
	mkdir $@
$(OBJ):
	mkdir $@
clean:
	rm -rf $(OBJ) $(BIN)
```

Numerous pattern substitutions are required to keep the build organised,
placing intermediate files neatly in temporary subdirectories, which can
be removed to clean the project. Building the project is a matter of
issuing the command `make` from the project root.

# Migrating to CMake

The process of compiling this project is more easily achieved with
CMake. The configuration for CMake resides in a number of `CMakeLists`
files (often `CMakeLists.txt` for MS Windows compatibility). The
directory structure with these files added:

```text
.
|-- CMakeLists.txt
|-- inc
|   |-- item.h
|   `-- list.h
|-- makefile
|-- run
|   |-- item_test.c
|   `-- list_test.c
`-- src
    |-- CMakeLists.txt
    |-- item.c
    `-- list.c
```

A `CMakeLists.txt` file contains a set of commands, all of them
documented copiously in CMake's
[reference](https://cmake.org/cmake/help/latest/index.html). The
configuration file in the project root begins with the lines:

```cmake
# ./CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(List LANGUAGES C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
```

This snippet specifies the minimum version of CMake required to build
the project, the name of the project itself and - optionally - the
language in use. I have additionally enabled
`CMAKE_EXPORT_COMPILE_COMMANDS`, which generates the
`compile_commands.json` file used by the language server _clangd_ to
resolve references across source files in the project.

The next step is defining the targets for the build, in this case the
two source files in the run directory. These files both reference header
files in the `./inc` directory and link the library defined in the
`./src` directory. Using CMake, the configuration for this library can
be managed separately and is very simple. On account of CMake scope
rules, the `CMakeLists.txt` files in nested subdirectories do not need
to repeat the commands used above and the library build can be expressed
with a single command:

```cmake
# src/CMakeLists.txt
add_library(src item.c list.c)
```

Specifying the executable targets, linking the library and setting the
include path is all handled in the main `./CMakeLists.txt` file.

```cmake
# ./CMakeLists.txt
add_executable(ItemTest "run/item_test.c")
add_executable(ListTest "run/list_test.c")
add_subdirectory(src)
foreach(executable ItemTest ListTest)
	target_link_libraries("${executable}" PUBLIC src)
	target_include_directories("${executable}" PUBLIC "{PROJECT_SOURCE_DIR}/inc" "${PROJECT_BINARY_DIR}")
endforeach()
```

Both executables are to be linked with the `./src` library, with the
`./inc` directory appearing in the include path during compilation. A
`foreach` loop is used to update both targets with these relationships.

# Building the Project

Using these two files, CMake has a model of all of the targets and
relationships involved in the build of the project. The original
makefile had the additional advantage of separating object files and
executables from the source code. The same can be achieved with CMake by
performing an _out-of-source_ build:

```text
$ mkdir build
$ cmake -B build
-- The C compiler identification is GNU 13.2.1
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Configuring done (0.1s)
-- Generating done (0.0s)
-- Build files have been written to: /path/to/project/build
$ cmake --build build
[ 14%] Building C object src/CMakeFiles/src.dir/item.c.o
[ 28%] Building C object src/CMakeFiles/src.dir/list.c.o
[ 42%] Linking C static library libsrc.a
[ 42%] Built target src
[ 57%] Building C object CMakeFiles/ItemTest.dir/run/item_test.c.o
[ 71%] Linking C executable ItemTest
[ 71%] Built target ItemTest
[ 85%] Building C object CMakeFiles/ListTest.dir/run/list_test.c.o
[100%] Linking C executable ListTest
[100%] Built target ListTest
```

By creating an isolated build directory, the object and executable files
generated during the build are conveniently separated from the source
tree. Finally, run the newly created binaries to verify that everything
is in order and then clean up by removing the build directory:

```text
$ ./build/ItemTest
"Hello!" 1234 'T'
$ ./build/ListTest
[4, 4, 5, 3.200000, 't', "Hello!", "Hello!", "Hi!"]
$ rm -rf build
```
