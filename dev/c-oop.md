---
author: Alexander Neville
date: 2023-08-02
title: Object Orientation in C
---

C and C++ are two system languages with support for dynamic memory
allocation. It is possible to allocate, address and access regions of
memory directly to build more complex types from scratch. _Abstract Data
Types_ are, however, more difficult to implement; information hiding,
encapsulation and other design patterns typically associated with object
orientation are not present in the C specification.

# Namespaces

C has no built in support for user defined namespaces, while C++ does.
This becomes problematic when two libraries, each with a separate header
file, contain functions with the same name and signature, causing a
multiple function definition error. For a library implemented in the
file `lib.c`, along with the `lib.h` header file containing function
signatures, an `extern struct` containing function pointers can be used
to provide the public interface to the library. The structure definition
is part of the same header file.

```c
// lib.h
struct lib {
    void (*F1) ();
    void (*F2) ();
};
extern struct lib Lib;
```

This header does not contain any function signatures, they are
effectively private and could be made static. Any file including `lib.h`
knows of the declaration of the variable `struct lib Lib`. This
definition takes place in the library source file, the function pointers
are set to the appropriate internal functions.

```c
// lib.c
#include "lib.h"
#include <stdio.h>
static void F1_ () {puts("F1");}
static void F2_ () {puts("F2");}
struct lib Lib = {.F1 = &F1_, .F2 = &F2_};
```

By including `lib.h`, any other source file can access the functions of
the library _namespace_ referred to by `Lib` with a syntax that
resembles many other high level languages.

```c
//  main.c
#include "lib.h"
int main() {Lib.F1(); Lib.F2();}
```

# Encapsulation

If the purpose of a source file or library is to define a type, it is
preferable that as little information about the type implementation is
exposed to the wider program. This is easily achieved in C by moving the
structure definition from the header file back to the source file,
leaving only the `typedef` alias in place. For example, a simple type
containing two integers and representing a two dimensional coordinate
system, would be defined outwardly with a type alias and a set of public
functions.

```c
// coord.h
typedef struct coord_S_ coord;
coord * new_coord(int x, int y);
void print_coord(coord * self);
void del_coord(coord * self);
```

The type definition in the header file is the only information made
available outside the source file implementing the structure itself.
Operations on the structure contents are restricted to functions
implemented in the same source file as the full structure definition,
which is the desired behaviour.

```c
// coord.c
#include "coord.h"
#include <stdio.h>
#include <stdlib.h>
struct coord_S_ {
    int x;
    int y;
};
coord * new_coord(int x, int y) {
    coord * new = (coord *) malloc(sizeof(coord));
    new->x = x;
    new->y = y;
    return new;
};
void print_coord(coord * self) {
    printf("(%d, %d)\n", self->x, self->y);
}
void del_coord(coord * self) {
    free(self);
}
```

Adopting this approach necessitates the dynamic allocation of the
_opaque_ type. It is impossible to create a new structure variable
automatically outside the type implementation, as the type definition is
incomplete and has no size.

```c
// main.c
#include "coord.h"
int main() {
    // coord a; // not valid, incomplete type definition
    coord * a = new_coord(3,2);
    print_coord(a);
    del_coord(a);
}
```

# Polymorphism

To create a polymorphic type, the behaviour of which depends on its
type, a structure must remember its type and act accordingly. A `union`
/ `enum` pair can be used to this end. The _union_ is an area of memory
large enough to store the biggest member and hence every smaller member,
although not at the same time. The _enumeration_ is a convenient way to
determine the type of the data value and hence how many bytes to read.

```c
union item_type_U_ {
    int    int_val;
    float  flt_val;
    double dbl_val;
    char   char_val;
    char * str_val;
    void * ptr_val;
};

enum item_type_E_ {
    Integer,
    Double,
    Float,
    Character,
    String,
    Pointer
};

struct item_T_ {
    item_val val;
    item_type type;
    char * repr;
    int repr_len;
    unsigned long hash;
    bool dync_memb;
};
```

Type aliases make driver code and function signatures more concise.
Additionally, type aliases can be forward declared to hide the structure
definition if required. For the sake of practicality, the structure
definition should remain visible, enabling an item to be stored in
place, without the need to be allocated or dereferenced. A type designed
to be allocated dynamically presents many difficulties, consistent with
the common drawbacks of explicit memory management, namely: ownership,
type safety, memory leaks, mutability etc.

```c
typedef union item_type_U_ item_val;
typedef enum item_type_E_ item_type;
typedef struct item_T_ item;
typedef struct item_T_ * item_p;
```

There are good reasons to handle the type as a pointer and a statically
allocated structure. Using forward declaration of the structure and type
alias, its implementation can be hidden, though this prevents an
allocated object in place, for the sake of accelerating a program. As it
is possible to obtain a pointer for an object later, this version of the
constructor returns a `struct` directly. The constructor is _variadic_,
only the first parameter, the type, is made explicit.

```c
item item_new(item_type type, ...) {
    va_list args;
    va_start(args, 1);
    item self;
    self.type = type;
    switch ( self.type ) {
        case Integer: self.val.int_val = va_arg(args, int); break;
        case Double: self.val.dbl_val = va_arg(args, double); break;
        case Float: self.val.flt_val = (float) va_arg(args, double); break;
        case Character: self.val.char_val = (char) va_arg(args, int); break;
        case String: self.val.str_val = va_arg(args, char *); break;
        case Pointer: self.val.ptr_val = va_arg(args, void *); break;
        default: break;
    }
    va_end(args);
    self.repr = NULL;
    item_dealloc_dynamic_members(&self);
    return self;
}
```

With this approach, a new item can be instantiated with the very elegant
`(type, value)` syntax, which is quite atypical in C.

```c
item b = item_new(Integer, 1234);
item_p c = item_clone_p(&b);
item_modify(c, Character, 'T');
char * string1 = item_repr(&b);
char * string2 = item_repr(c);
printf("%s %s\n", string1, string2); // 1234 'T'
free(string1); free(string2); free(c);
```

The complete source file can be downloaded
[here](https://cdn.afneville.com/files/item.c).

# Object Methods

Object orientated languages typically use _dot-qualifier_ syntax for
accessing object methods. This is a helpful organisational feature and
design pattern. This can be emulated in C with function pointers. Here
is an example of a structure which has data fields and a list of
function pointers. These act as the methods of the improvised _object_.

```c
typedef struct matrix_T {
    float * array;
    int x, y;
    // methods
    struct matrix_T * (* scale_matrix)  (struct matrix_T * matrix_p, float scale_factor, bool adjust);
    struct matrix_T * (* select_region) (struct matrix_T * matrix_p, int x, int y, int w, int h);
    struct matrix_T * (* horiz_density) (struct matrix_T * matrix_p);
    struct matrix_T * (* vert_density)  (struct matrix_T * matrix_p);
    float (* average_darkness)          (struct matrix_T * matrix_p);
    struct matrix_T * (* paste) (struct matrix_T * fg, struct matrix_T * bg);
    struct matrix_T * (* translation) (struct matrix_T * matrix_p, int x_offset, int y_offset);
} matrix;
```

A function corresponding to each of these _methods_ are declared and
implemented in the usual fashion. Their signatures align with those of
the associated method, though their names need not necessarily match (in
this case, for clarity, they do).

```c
matrix * scale_matrix(matrix * matrix_p, float scale_factor, bool adjust);
matrix * select_region(matrix * matrix_p, int x, int y, int w, int h);
matrix * horiz_density(matrix * matrix_p);
matrix * vert_density(matrix * matrix_p);
float average_darkness(matrix * matrix_p);
matrix * paste(matrix* fg, matrix * bg);
matrix * translation(matrix * matrix_p, int x_offset, int y_offset);
```

In the constructor, each method is assigned the address of a function.

```c
matrix *
create_matrix(int height, int width)
{
  matrix *new_matrix_p = (matrix *) malloc(sizeof(matrix));
  new_matrix_p->array = (float *) malloc(sizeof(float) * height * width);
  new_matrix_p->x = width;
  new_matrix_p->y = height;
  new_matrix_p->scale_matrix = &scale_matrix;
  new_matrix_p->select_region = &select_region;
  new_matrix_p->average_darkness = &average_darkness;
  new_matrix_p->horiz_density = &horiz_density;
  new_matrix_p->vert_density = &vert_density;
  new_matrix_p->paste = &paste;
  new_matrix_p->translation = &translation;
  return new_matrix_p;
}
```

The constructor is defined in the same source file as the method
implementations. The signatures of those functions can be withheld from
the calling program by omitting them in the header file. Only the
`matrix` structure and its fields are visible to the call site. Now the
methods of the `matrix` _object_ are only accessible via the structure
itself.

```c
matrix * m1 = create_matrix(100, 100);
// ...
matrix * m2 = m1->scale_matrix(m1);
// ...
```

The complete source file can be downloaded
[here](https://cdn.afneville.com/files/matrix.c).
