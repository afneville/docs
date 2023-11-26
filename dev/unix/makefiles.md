---
date: 2023-11-11
author: Alexander Neville
title: Writing Makefiles
---

`make` is a utility designed to automatically rebuild parts of a program
when its components are modified. `make` refers to a user-defined
_makefile_ to identify the relationships between files. If the time
stamp of a file is newer than that of another file that depends on it,
`make` will run the specified actions to rebuild the output from the
specified inputs. `make` is often used in the build process of a C
program, but can be used to great effect automating tasks in which an
output file depends on a number of other files and should be rebuilt
when they change.

# Rules

A makefile is composed of _rules_, amongst other directives. A rule has
a _target_ (which may be one or more files), an optional list of
_prerequisites_ and an optional _recipe_.

```makefile
target: prerequisites
    recipe
```

- A target is usually the name of a file to create or update, though it
  may be a named action, depending on some files. Such a target is
  considered _phony_.
- A prerequisite is a file that the target depends on. If any
  prerequisite has a more recent time stamp than the target, the recipe
  is executed, except in the case of an _order-only_ prerequisite.
- A recipe is the action to be executed if the target is out of date.
  Usually the recipe will update the target. The lines of a recipe must
  begin with a tab character.

Consequently, a rule can be said to set out when and how a target should
be updated. By default, the first named target is built when make is
invoked. This target is considered a _goal_. `make` will try to build a
goal by building its prerequisites, which may themselves be targets or
another rule.

## Explicit Rules

An explicit rule is as above. Explicit rules are not provided by `make`
by default, as is the case with implicit rules. An explicit rule may
have more than one target, in which case the targets are treated as
_independent_ or _grouped_. A target may feature in multiple explicit
rules, though preferably only one recipe is provided for a target.

## Implicit Rules

Implicit rules capture typical use cases and define rules for a class or
set of files, often those with a common extension. `make` provides a
catalogue of built-in implicit rules. It is possible create new implicit
rules in addition to overriding and cancelling built-in implicit rules.
The `-r` flag can be used to cancel all such built-in rules. The section
of the `make` manual page detailing the list of built-in implicit rules
is
[here](https://www.gnu.org/software/make/manual/make.html#Catalogue-of-Rules).

A common example of an implicit rule is the compilation of a C source
file. In this example, the makefile only explicitly details how to
create a target, called `app` from the prerequisite `app.o`. No rule is
provided in this makefile for creating the object file.

```makefile
app: app.o
	cc -o app app.o
```

`make` recognises that object files are often generated from a
correspondingly named `.c` source file and provides a recipe to build
it.

```language-plaintext
$ ls
app.c  makefile
$ make
cc    -c -o app.o app.c
cc -o app app.o
$ ls
app  app.c  app.o  makefile
```

## Pattern Rules

A _pattern rule_ may be used to define a new implicit rule. A pattern
rule contains exactly one `%` character in the target. The `%` character
matches one or more characters. The string matched by the `%` expression
is called the _stem_. Should `%` occur in the prerequisites, it is
replaced by the stem from the target. A substitution does not need to
occur in the prerequisites of a pattern-rule, nor does a pattern rule
need to have any prerequisites or even a recipe. A pattern rule is
likely to use an _automatic variable_ in its recipe. In this case `$@`
matches the name of the target that caused the rule to be run and `$<`
is the name of the first prerequisite.

```makefile
%.o: %.c
	cc -c $< -o $@
app: app.o
	cc -o app app.o
```

This pattern rule overrides the implicit rule for C source file
compilation.

## Static Pattern Rules

Static pattern rules are analogous to implicit rules for a specified set
of files. The syntax is
`targets: target-pattern: prerequisite-pattern; recipe`. Here is an
example, using automatic variables and static pattern matching to
override the implicit rule for generating an object file for the files
`app.o` and `lib.o`.

```
app: app.o lib.o
	cc -o $@ $^
app.o lib.o: %.o: %.c; cc -c -o $@ $<
```

## Double-Colon Rules

An uncommon type of rule is the _double-colon_ form, resembling
`target :: prerequisites ; recipe`. A target must appear in only
double-colon or only normal rules, not both. The target of such a rule
is updated if any of its prerequisites are newer than it, but unusually
a target without prerequisites is always updated. Many double-colon
rules may exist for the same target, including a recipe. This rule is
used in the rare case that the recipe to update a target depends on
which prerequisites have changed.

```language-plaintext
$ ls
app.c  makefile
$ make
cc -c app.c -o app.o
cc -o app app.o
```

## PHONY Targets

If the first named target happens to be a named action rather than a
file to be built, this could cause some undesired behaviour. In the
example given here, the `clean` target is written first.

```makefile
clean:
	- rm -f target
	- rm -f prerequisite
target: prerequisite
	cp prerequisite target
prerequisite: source
	cp source prerequisite
```

If `make` were invoked without an argument and the file `clean` did not
exist, this makefile would cause the clean up recipe to run. This
example demonstrates the use of a hyphen at the beginning of a recipe
line. The hyphen is stripped by `make` before the line is executed and
`make` will ignore any errors generated by executing the command.

```language-plaintext
$ ls
makefile  source
$ make
rm -f target
rm -f prerequisite
$
```

Named actions without any prerequisites, such as `clean`, should not be
executed unless the name of the target is passed as an argument. Moving
the `clean` rule to the end of the file may be an adequate solution to
this problem; the recipe for the rule named `target` would be executed
instead, which depends on `prerequisite`, which in turn depends on the
file named `source`.

```makefile
target: prerequisite
	cp prerequisite target
prerequisite: source
	cp source prerequisite
clean:
	- rm -f target
	- rm -f prerequisite
```

The `clean` target is not a prerequisite of any rule that is used to
build the file `target` and therefore it does not run. It can be run by
passing its name as an argument to `make`.

```language-plaintext
$ ls
makefile  source
$ make
cp source prerequisite
cp prerequisite target
$ ls
makefile  prerequisite  source  target
$ make clean
rm -f target
rm -f prerequisite
$ ls
makefile  source
```

This is not a perfect solution, however. If a file named `clean` exists,
then the rule will not be run.

```language-plaintext
$ ls
makefile  source
$ touch clean
$ ls
clean  makefile  source
$ make clean
make: 'clean' is up to date.
```

The builtin `.PHONY` target can be used to express that the rule named
`clean` does not refer to a file and its recipe will not build the
target. This example also illustrates that rules beginning with a single
`.` will not be considered a default target, unless followed immediately
by a `/`.

```makefile
.PHONY: clean
./target: prerequisite
	cp prerequisite target
prerequisite: source
	cp source prerequisite
clean:
	- rm -f target
	- rm -f prerequisite
```

This version of the makefile behaves as intended, building files named
`target` and `prerequisite` from the file `source` and running the
`clean` target when `make` is explicitly passed the name of the rule,
even in the presence of a file matching its name.

```language-plaintext
$ ls
clean  makefile  source
$ make
cp source prerequisite
cp prerequisite target
$ ls
clean  makefile  prerequisite  source  target
$ make clean
rm -f target
rm -f prerequisite
$ ls
clean  makefile  source
```

Action rules can be handled another way: using another rule without
prerequisites or a recipe. The target of this rule is given as a
prerequisite of the action, causing the target to be updated and
consequently the action to be run.

```makefile
./target: prerequisite
	cp prerequisite target
prerequisite: source
	cp source prerequisite
clean: FORCE
	- rm -f target
	- rm -f prerequisite
FORCE:
```

## Multiple Targets

An explicit rule may have more than one target. If the usual
`target: prerequisites; recipe` syntax is used, then the targets are
considered independent. The compact rule may be separated into a number
of rules with common prerequisites and recipes. The automatic variable
`$@` may be used to access the target that triggered the rule.

```makefile
app: app.o lib.o
	cc -o app app.o lib.o
app.o lib.o: lib.h
# ^ is equivalent to:
# app.o: lib.h
# lib.o: lib.h
```

This examples states that both object files are dependent on a common
header file and changes to it should result in the re-compilation of the
source files. In this example, the implicit rule to do so is used.

```language-plaintext
$ ls
app.c  lib.c  lib.h  makefile
$ make
cc    -c -o app.o app.c
cc    -c -o lib.o lib.c
cc -o app app.o lib.o
$ ls
app  app.c  app.o  lib.c  lib.h  lib.o  makefile
$ touch lib.h
$ make
cc    -c -o app.o app.c
cc    -c -o lib.o lib.c
cc -o app app.o lib.o
```

If `&:` is used in place of `:`, a number of targets is considered
grouped. The recipe is assumed to update all of the targets and the rule
is triggered if any of the targets are out of date.

```makefile
one two &: prerequisite
	touch one
	touch two
```

The example invocation below demonstrates that rule is run if just one
of the prerequisites is out of date.

```language-plaintext
$ ls
makefile  prerequisite
$ make
touch one
touch two
$ touch prerequisite
$ touch two
$ make
touch one
touch two
```

## Order-only Prerequisites

A recipe is usually run if any of a rule's prerequisites are newer than
the target. More often than not, this is the desired behaviour, though
in rare cases one may want a recipe to run only if the prerequisite does
not exist and not in the case that the prerequisite is newer than the
target. Such a prerequisite is called an _order-only_ prerequisite.
These prerequisites are placed after all (possibly zero) normal
prerequisites, separated by a `|` character.

```makefile
target: | directory
	touch target
directory:
	mkdir -p directory
```

In this example, the recipe for updating the filed named `target` is not
run when the prerequisite `directory` exists, despite the prerequisite
being newer.

```language-plaintext
$ ls
makefile
$ make
mkdir -p directory
touch target
$ ls
directory  makefile  target
$ touch directory
$ make
make: 'target' is up to date.
```

# Variables

A variable is a stand-in for a string of text, known as the variable's
value. The values of variables may be used in any part of a makefile and
can be substituted with the `$(variable-name)` syntax.
`${variable-name}` is also acceptable. Variables may be redefined
repeatedly and its value reflects that of it's most recent definition.
The `$` character may be escaped by prefacing with another `$`.

## Variable Flavours

The expansion of a variable's value may occur at the point of definition
or the point of substitution. The difference between the two _flavours_
is detailed here.

The single `=` sign can be used for variable assignment. This form is
called _recursively-expanded_ variable assignment. The value of the
variable is expanded at the point of substitution. In the given example,
`make recursive` will print _two_, as the value of `a` is expanded in
the rule, where the most recent definition of `b` is _two_.

```makefile
b = one
a = $(b)
b = two
recursive:
	@echo $(a) # two
```

The `:=` and `::=` forms are identical and called _simply-expanded_
variable assignment. The behaviour of this example is more predictable,
as the value of `x` is expanded at the point of definition, at which
point the value of `y` is _one_.

```makefile
y = one
x := $(y)
y = two
simple:
	@echo $(x) # one
```

## Appending to a Variable's Value

Values may additionally be assigned to variables using the `+=` form,
which appends a value to a variable. If the variable has not previously
been defined, it acts as `=`. If the variable was previously assigned a
value, then the behaviour depends on how that value was assigned.

If the variable was defined as simply-expanded, then `+=` is equivalent
to:

```makefile
var := initial
var := $(var) appended
```

The following example consequently echoes _one two_.

```makefile
a = one
b := $(a)
b += two
a = three

simple:
	@echo $(b) # one two
```

In the case of recursively-expanded variables, `+=` is roughly
equivalent to:

```makefile
var = initial
tmp = $(var)
var = $(tmp) appended
```

Consequently, this example outputs _three two_.

```makefile
a = one
b = $(a)
b += two
a = three

recursive:
	@echo $(b) # three two
```

## Multi-Line Variables

Multi-line variables are easily set with the `define` directive.

```makefile
variable = target
define recipe :=
touch target
echo $(variable)
endef
variable = not observed

target:
	$(recipe)
```

The `define` directive is followed by the name of the variable and the
type of assignment. If the latter is omitted, it is assumed to be `=`.

## Automatic Variables

Automatic variables are computed automatically by `make` for each rule.
Their values are not consistent between rules and their values depend on
the targets and prerequisites, in which they may not be used. Complete
reference for the automatic variables can be found
[here](https://www.gnu.org/software/make/manual/make.html#Automatic-Variables).
The most often-used automatic variables are listed here.

- `$@`: The name of the target, or the name of the target that caused
  the recipe to be run in a rule with multiple targets.
- `$<`: The name of the first prerequisite.
- `$^` The names of all the prerequisites, separated by spaces.
- `$^` The names of all the prerequisites newer than the target,
  separated by spaces.
- `$|` The names of all the order-only prerequisites, separated by
  spaces.

All of these examples are only a single character long and hence the
brackets may be omitted; `$(@)` is equally valid.

# Conditionals

Conditional expressions are used to compare variables to value and
conditionally include or ignore parts of a makefile. Conditionals are
textual features and `make` effectively does not parse sections of the
makefile that follow the false branch of a conditional, if it exists.

A conditional is formed by a _conditional directive_ followed by any
number of lines that are included when the condition is true; any number
of _else_ conditional directives (or simply else without a conditional
directive) and the mandatory `endif` directive.

In this example, value _Success_ is assigned to to the variable _output_
if the value of _input_ is 0. Note the absence of an else statement,
which is optional.

```makefile
input = 0
output = Error
ifeq ($(input),0)
output = Success
endif

target:
	@echo $(output)
```

Equally, any number of else conditional directives may appear,

```makefile
input = 2
ifeq ($(input),0)
output = none
else ifeq ($(input),1)
output = one
else ifeq ($(input),2)
output = couple
else
output = several
endif

target:
	@echo $(output) # couple
```

Other conditional directives include `ifneq`, obviously the opposite of
`ifeq`; `ifdef`, which determines whether a variable name has a
non-empty value; and `ifndef`, which is the opposite of `ifdef`.

# Functions

Function calls resemble variable references and may be substituted in
the same places. Functions additionally accept arguments and are written
`$(function arg,arg,arg)`. Curly brackets are of course acceptable as
well. A function's arguments are separated from its name by a single
white space character and multiple arguments are separated by commas.
These two characters may need to be hidden within a variable, if they
appear as an argument.

Complete function reference can be found
[here](https://www.gnu.org/software/make/manual/make.html#Functions).
