---
author: Alexander Neville
date: 2024-01-15
title: Shell Scripting
description: An introduction to shell scripting.
path: dev/unix/shell
image-meta: bash-logo.png
image-meta-credit: https://github.com/odb/official-bash-logo
---

A shell script is a program that is executed by a shell. Shells operate
as command line interpreters; scripts are an automated method of issuing
sequential commands to a shell. Shell scripts are plain text files and
are not compiled before they are executed. They must, however, have
their executable bit set.

# Shebang

If a plain text file is executed as a command and the first line begins
`#!`, the remainder of the first line is treated as the _interpreter
directive_. The specified interpreter is passed the path of the executed
file as an argument. This simplifies the command line execution of a
script and obscures or abstracts details such as the intended
interpreter.

The interpreter specified in the directive may be a compiled executable
or another script following the same pattern. Common examples include:

- `#!/bin/sh`
- `#!/bin/bash`
- `#!/usr/bin/env bash`

Very nearly every Unix system will make `/bin/sh` some POSIX compliant
shell. If a script requires certain features not present in POSIX sh, a
more explicit shebang should be selected. Some non-GNU systems will not
have bash installed and it is of no guarantee that bash is installed as
`/bin/bash`. Equally, the `env` program and its path are not universal.

To make a file executable add the appropriate permission. Run the newly
created executable by putting its path at the command line.

```sh
chmod u+x script.sh
./script.sh
```

# Variables

Shell variables may contain characters, strings of characters or
numbers. Shell has no concept of data types, all variables are really
strings, but some may be treated as numbers by external programs.
Variables are created when a value is assigned to a name. Variable names
may contain letters and the `_` symbol. Single `=` is used for
assignment. Variable names and values may not be space separated during
assignment. Quotes are required to preserve white space in strings.

```sh
letters=hello
words="hello    world"
number=5
echo $words
echo "$words"
```

To separate a variable name from string contents, `${}` syntax is used.

```sh
firstWord="Hello"
greeting="${firstWord}, world!"
echo "$greeting"
```

The output of a command can be included in a string with _command
substitution_. `$()` is the preferred syntax, though double back-ticks
have the same effect.

```sh
echo "uptime: $(uptime)"
echo "uptime: `uptime`"
```

Variables are not declared before assignment or use. Attempting to use
the value of a variable that has not been assigned to results in the
empty string, not an error.

The following script is named `script.sh`.

```sh
#!/bin/sh
echo "variable value: $variable" #
variable="updated value"
echo "variable value: $variable" # updated value
```

Executing the script in an interactive shell will result in the
following output.

```language-plaintext
$ ./script.sh
variable value:
variable value: updated value
```

The value of `variable` in the script's output remains empty in the case
that `variable` is declared in the interactive shell before the script
is executed.

```language-plaintext
$ variable="initial value"
$ echo "variable"
variable
$ echo "$variable"
initial value
$ ./script.sh
variable value:
variable value: updated value
```

When the script is executed, a new shell is created according to the
interpreter directive. The value of `variable` is not set in this new
environment. To make the script inherit the value of a variable in the
parent process, the variable must be _exported_.

```language-plaintext
$ variable="initial value"
$ export variable
$ ./script.sh
variable value: initial value
variable value: updated value
$ echo "$variable"
initial value
```

Assignments to `variable` within the script are not reflected in the
parent process after the script terminates. If this behaviour is desired
the script should be _sourced_ within the interactive shell. The
`source` or `.` built-ins are the way this is achieved.

```language-plaintext
$ variable="initial value"
$ . ./script.sh
variable value: initial value
variable value: updated value
$ echo "$variable"
updated value
```

In shell, some variables are set automatically and should not be
assigned to.

```sh
#!/bin/sh
echo "$#"
echo "$0"
echo "$@"
```

In this script, the special variables are:

- `$#` is the number of arguments passed to the script.
- `$@` is a space delimited string of all the arguments passed to the
  script
- `$0` is the name of the script as it was called.
- `$1` to `$9` are are the first 9 optional parameters.

Evaluated in an interactive shell, this script outputs the following:

```language-plaintext
$ ./script.sh hello my name is alex
5
./script.sh
hello my name is alex
```

Some other special variables:

- `$$` is the PID of the current shell.
- `$!` is the PID of the last run background process.
- `$?` is the exit code of the last executed command.

# Arrays

Arrays are named in the same fashion as variables. An array is written
as a list of space-delimited values inside `()` brackets.

```sh
myArray=(Hello, "my name" is Alex)
echo ${myArray[@]}  # Hello, my name is Alex
echo ${myArray[1]}  # "my name"
echo ${#myArray[@]} # 4
```

# Arithmetic Operations

Use `$(())` syntax for arithmetic operations. The operators are as one
might expect.

```sh
a=3
b=$((a+5))
b=$(($a+5))
echo $b # 8
8
echo $(($b / 5)) # 1
echo $(($b % 5)) # 3
```

# Loops

As in many other languages, for loops are pre-determined and iterate
though a set of values.

```sh
#!/bin/sh
for i in 1 2 3 4 5
do
  printf "$i "
done
echo # prints 1 2 3 4 5
```

While loops too are exactly as one would expect. `:` always evaluates to
true.

```sh
#!/bin/sh
while :
do
  echo "hello"
done
```

# Test

In shell scripting, the external program `test` is used to evaluate
conditional expressions. It is often invoked as `[`. When called like
this, test expects its argument list to be terminated with `]`. As with
any program, arguments passed to test must be space separated. The
`test` manual page lists its syntax and options.

The `test` program sets its exit code according to the result of the
condition. A `0` exit code indicates the expression is true, a `1`
indicates the expression is false, in keeping with the exits success
convention on Unix.

```sh
#!/bin/sh
[ "hello" = "hello" ]
echo $? # 0
[ "hello" = "world" ]
echo $? # 1
```

This value can be checked in one line.

```sh
#!/bin/sh
[ "hello" = "hello" ] && echo "strings match"
[ "hello" = "world" ] || echo "strings don't match"
```

For complicated behaviour, it is best to wrap the conditional statements
in `if`/`then`/`fi`. Each of these keywords must appear on new lines, or
separated by semi-colons.

```sh
#!/bin/sh
if [ 1 -eq 1 ]
then
    echo "The expression is true."
fi
```

# Case

The case statement is intuitive. The following script prints `Two`.

```sh
#!/bin/sh

x=2

case $x in
    1) echo "One" ;;
    2) echo "Two" ;;
    3) echo "Three" ;;
    *) echo "Larger Than Three" ;;
esac
```

# Functions

The scope rules of functions in shell are unusual. Consider the
following example:

```sh
#!/bin/sh

testfunction() {
    echo "Function arguments: " "$@"
    X=2
}

X=1
echo "X: $X"
testfunction 1 2 3
echo "Script arguments: " "$@"
echo "X: $X"
```

Named `test.sh` and called with the arguments `a b c`, the function is
demonstrated to have a different argument array to the main script, but
the variable `X` is global.

```text
$ ./test.sh a b c
X: 1
Function arguments:  1 2 3
Script arguments:  a b c
X: 2
```
