---
title: Working with Java Packages
author: Alexander Neville
date: 2024-01-11
description:
  An overview of the concepts and conventions that govern Java packages.
path: dev/java-packages
---

Java packages provide a mechanism to group related types, control access
to members and provide namespaces. Package naming convention is a
contentious subject and can be the source of much confusion and many
errors, particularly surrounding the _classpath_.

# Creating a Package

Public package members are placed in a source file, the first line of
which contains a `package` statement. Non-public types may be included
in the same file as a public type. Any type that is not public will be
package-private. Source and class files that belong to a package are
expected to be placed in a directory structure mirroring the package
name.

Consider this project layout:

```text
.
|-- example
|   |-- One.java
|   `-- Two.java
`-- Main.java
```

The classes `One` and `Two` belong to the `example` package; they both
contain a package statement and reside in a correspondingly named
directory. In this case, the relative path of the source files matches
the _fully-qualified_ name of the classes - permissible
because the present working directory is conventionally an entry in the
[classpath](#classpath).

```java
// example/One.java
package example;

public class One {
    public static void func() {
        System.out.println("example.One");
    }
}


// example/Two.java
package example;

public class Two {
    public static void func() {
        System.out.println("example.Two");
    }
}
```

## Accessing Package Members

A package member can be accessed with its fully-qualified name. It's
worth noting that any class that does not explicitly belong to a
particular package is assigned to a default unnamed package.

```java
// Main.Java
public class Main {
    public static void main(String[] args) {
        example.One.func();
        example.Two.func();
    }
}
```

If a particular type is used frequently, it may be sensible to _import_
it into the current namespace. This is useful for writing succinct code,
though introduces the possibility of a name collision, as Java's
`import` statement does not support name remapping as Python's does, for
example. When two identically named classes from separate packages are
used simultaneously, the convention is to use the qualified name of at
least one of the types, possibly importing the more frequently used of
the two.

```java
import example.One;
import static example.Two.func;

public class Main {
    public static void main(String[] args) {
        One.func();
        func();
    }
}
```

The Java `import` statement only supports types, not packages, so
`import example;` is erroneous. The _static import_ statement is not
often used, because its use often introduces ambiguity. Its ability to
import a static final member of a type into the current file is
demonstrated here. All of the types in a package can be imported using
the `import packagename.*;` form.

# Classpath

The Java _classpath_ is analogous to the system `PATH` environment
variable, it is a list of directories and archives which contain Java
packages. The list of entries in the classpath is traversed to search
for a package exhaustively.

In this example, the class `Main` belongs to the package `app`. All of
the compiled sources have been placed in a dedicated directory within
the project, in this case called `out`.

```text
.
`-- out
    `-- app
        `-- Main.class
```

Attempting to execute the `Main` class from the root of this directory
hierarchy will result in an error: the specified class cannot be found.

```text
$ java app.Main
Error: Could not find or load main class app.Main
Caused by: java.lang.ClassNotFoundException: app.Main
```

As the present working directory is conventionally part of the
classpath, the `Main` class can be executed without modifying the
classpath from the `out` directory, from which the qualified name of the
class matches its relative path.

```text
$ pushd out && java app.Main && popd
Hello, World!
```

Alternatively, the classpath can be modified. The classpath may be set
via an environment variable or as a command line option.

```text
$ java -cp 'out' app.Main
Hello, World!
$ export CLASSPATH='out'
$ java app.Main
Hello, World!
```

The classpath may contain more than one entry. This enables packages to
be installed in various locations throughout a file system for use
across projects. In this example, the `app.Main` class depends on the
`mylib.Helper` class.

```text
.
|-- libs
|   `-- mylib
|       `-- Helper.class
`-- out
    `-- app
        `-- Main.class
```

With the classpath set to `'out'`, a runtime error predictably occurs
when the file attempts to access the class `mylib.Helper`.

```text
$ java -cp 'out' app.Main
Hello, World!
Exception in thread "main" java.lang.NoClassDefFoundError: mylib/Helper
        at app.Main.main(Main.java:9)
Caused by: java.lang.ClassNotFoundException: mylib.Helper
        at java.base/jdk.internal.loader.BuiltinClassLoader.loadClass(BuiltinClassLoader.java:641)
        at java.base/jdk.internal.loader.ClassLoaders$AppClassLoader.loadClass(ClassLoaders.java:188)
        at java.base/java.lang.ClassLoader.loadClass(ClassLoader.java:526)
        ... 1 more
```

The solution is very simple: add the `libs` directory, in which the
package resides, to the classpath.

```text
$ java -cp 'out:libs' app.Main
Hello, World!
Code from mylib.Helper
```

# Naming Conventions

A number of pervasive naming conventions are observed in Java
development. Packages will often use reverse domain name qualification,
e.g. `com.afneville.mylib`, avoiding name collisions by providing a
namespace for individual developers and organisations. If another
developer were to create a package called `mylib`, they would precede it
with their own reverse domain name.

# Package Hierarchies

Java packages, despite how it may appear, are not structured
hierarchically. Apparent _subpackages_ are entirely distinct packages,
the naming convention only serves an organisational purpose.

Extending the example seen earlier, a package named `example.further` is
introduced alongside the other sources:

```text
.
|-- example
|   |-- further
|   |   `-- Three.java
|   |-- One.java
|   `-- Two.java
`-- Main.java
```

It is not unusual to replicate the package directory structure and
define the package sources elsewhere, updating the classpath
accordingly. In many cases, the development of a single package could be
treated as a separate project.

```text
.
|-- example
|   |-- One.java
|   `-- Two.java
|-- libs
|   `-- example
|       `-- further
|           `-- Three.java
`-- Main.java
```

In either case, the contents of the source file are unchanged.

```java
// (libs/)example/further/Three.java
package example.further;

public class Three {
    public static void func() {
        System.out.println("example.further.Three");
    }
}
```

Similarly, the means of accessing the package member is unaffected by
whichever directory layout is elected. Splitting the packages into
separate directories demonstrates how Java packages are independent of
the underlying hierarchical file system. Both import statements in the
`Main` class are required; importing all members of the `example`
packages will not import the `example.further` package or any of its
members.

```java
import example.*;
import example.further.*; // required

public class Main {
    public static void main(String[] args) {
        One.func();
        Two.func();
        Three.func();
    }
}
```

# Creating a Distributable JAR File

Java's package mechanism facilitates modularisation, organisation and reuse.
Useful packages can be distributed and used by others so long as they
can be located via the classpath. Consider this package:

```text
.
`-- example
    |-- One.java
    `-- Two.java
```

The sources can be distributed as is, but there are many compelling
reasons to compile sources beforehand. A directory containing a set of
packages as source or class files can be copied around as a crude means
of distribution. For the sake of convenience, a set of files can added
to an archive file. The `jar` utility supports creating and inspecting
archives.

```text
$ javac example/*.java
$ jar cf example.jar example/*.class
$ jar tf example.jar
META-INF/
META-INF/MANIFEST.MF
example/One.class
example/Two.class
```

With the classes packed into a JAR, a Java application can access the
package so long as the JAR is added to the classpath.

```text
$ ls
example.jar  Main.java
$ javac -cp 'example.jar:.' Main.java
$ java -cp 'example.jar:.' Main
example.One
example.Two
```
