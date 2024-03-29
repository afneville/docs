---
author: Alexander Neville
date: 2023-01-10
title: Lists
---

A _list_ is an abstract data type representing a finite number of
ordered values in which the same value may appear more than once. Lists
are frequently implemented with array and linked list data structures,
among others.

# Arrays

An array is a simple data structure which stores items in sequential
memory locations. Arrays can be written as a collection of items inside
square brackets as follows, $[4,3,7,2,9,1,8,6]$. This array has 8
elements, so its _size_ would be considered 8. If this array was
assigned to a variable $a$, its elements can be accessed through their
_index_ $i$ and the indexing notation $a[i]$. Indexing usually begins
from 0, so the valid indices for this list are $0 \ldots 7$, as seen in
figure [@fig:array].

![An array and its indices](../../res/array.svg){#fig:array}

Each element may be accessed _sequentially_ by incrementing or
decrementing the index as required, or at random by taking any index -
assuming it is a valid index. For the indexing operation to be
effective, each item in the array is required to be the same size, which
means in practice array elements are of the same type. In a language
such as C, the array $a$ would be considered a pointer to the memory
location of the beginning of the array and the index $i$ is an offset
from the start of the array. This means that $a[0]$ points to the
beginning of the array, the _zeroth_ element. For non zero indices, this
offset is calculated by multiplying the index by a certain number of
bytes equal to the size of each item, $c$. As such the address of $a[i]$
is $a + c \times i$.

# Arrays, Iteration & Invariants

Index access to array elements, makes the array data structure conducive
to sequential access. Iteration over the index variable, which is
repeatedly incremented, enables array elements to be accessed and
processed at run time. A _loop_ is the programming construct
facilitating iteration. It is commonly expressed in pseudocode and C
like syntax.

```text
For i <- 0 ... (N - 1) Do
    use value i

for (i = 0; i < N; i++) {
    // use value i
}
```

In both of these examples $N$ is an invariant. It does not change in the
loop body or anywhere else in the program. Invariants prevent common
errors such as accessing indices beyond the end of an array. Due to the
static nature of arrays (arrays are allocated once at a certain size),
array size is a common program invariant.

# List ADT

As an abstract data type, a list is defined in terms of its public
functions. A minimal list type can be defined with two _constructors_,
where $E$ is an element of a list and $L$ is a list.

- `emptylist() -> L`
- `prepend(E, L) -> L`

Any list can be created from a single empty list and a series of prepend
operations. The list $[1,2,3]$ is created with the expression
`prepend(1, prepend(2, prepend 3, emptylist()))`. This is called an
_inductive_ type definition, relying on the repeated application of the
inductive step `prepend(E,L)` on the base case `emptylist()`. Any useful
data type has accessor methods. In the absence of array indices, any
list element can be retrieved with a combination of two _selectors_.

- `head (L) -> E`
- `tail (L) -> L`

The functions head and tail are not defined for the empty list. An
additional function is required to determine whether a given list is
empty.

- `isemptylist (L) -> T|F`

With this set of functions, the following expressions are true.

- `isemptylist (emptylist ())`
- `not isemptylist (prepend(e, l))`
- `head (prepend(e, l)) == e`
- `tail (prepend(e, l)) == l`

# Recursion & Derived List Procedures

Iteration is the logical and convenient method to process a collection
of elements in an array, due to indexing. With the abstract list
definition and in many list implementations, such as linked lists, there
is no index. It becomes more convenient to process lists with recursion,
although any recursion can be expressed as an iteration. Because of the
inductive construction of the list type, obtaining the last element
requires every element in the list is processed or _traversed_. The
function `last` returns the last element of the list `l`.

```text
last(L:l) -> E {
    if (isemptylist(tail(l)) return head(l);
    return last(tail(l));
}
```

This implementation ensures that the `last` function is not recursively
applied to an empty list, although if the function is initially called
on an empty list, `tail` will be passed an empty list, for which it is
undefined. This situation could be handled with an additional condition
in the `last` function.

```text
last(L:l) ->E {
    if (isemptylist(l)) {
        error();
    } else if (isemptylist(tail(l)) {
        return head(l);
    } else {
        return last(tail(l));
    }
}
```

Appending, rather than prepending, to a list is also a derived function
on this list. For the sake of simplicity, appending a single element `x`
to the end of a list `l1` can be achieved by passing
`prepend(x,emptylist())` in place of `l2`.

```text
append(L:l1,L:l2) -> L {
   if (isemptylist(l1)) return l2;
   return prepend(head(l1), append(tail(l1), l2))
}
```

Modifying the base case slightly, it is possible to write a function
which appends a single element to the end of a list.

```text
append(L:l,E:x) -> L {
   if (isemptylist(l)) return prepend(x, emptylist());
   return prepend(head(l), append(tail(l), x))
}
```

Many of these derived functions are slow and inefficient. In practice it
might be easier to use some of the underlying implementation details of
a data structure to accelerate more complicated operations and expose
more functions on a type than are technically required. Also absent in
this ADT are _mutator_ functions, which destructively modify a list. In
the functional inductive approach to ADT specification lists are
immutable and each function returns a new list. It is convenient to
return a new immutable list for every operation on a given list, as it
makes a program safe and predictable. There is no concept of state that
can be modified during the course of the program, so applying the same
function on the same set of arguments will produce the same output. The
trade off is the space and time complexity associated with repeated
memory allocation and traversal, yet another reason why complex data
types in programming languages and libraries are truly abstract.

# Linked Lists

Lists contain a finite number of elements, but theoretically this number
has no upper bound. Lists on computers are practically limited in size
by the amount of space or memory available. A list type must allocate
space for the data elements it contains. If the maximum size of a list
is known in advance, an array may be the most effective way too
implement a list. Otherwise, if the size of a list varies at run time, a
more dynamic implementation is required. The first such implementation
is the _linked list_.

The most simple linked list is composed of a sequence of _nodes_ or
_two-cells_. Each contains an element (or a reference to an element) and
a _reference_ to the next node. Figure [@fig:abstract_llist] is the most
abstract graphical representation of the list $[8,4,1,7,3,6]$. The
second of each two-cell is a reference to the next node, not the first
cell of the next node, as is often depicted.

![An abstract graphical linked list
illustration](../../res/abstract_llist.svg){#fig:abstract_llist}

In any implementation, the nodes of a linked list are themselves a type,
in most cases hidden from the external interface of a linked list. The
fields or cells of a node may contain a reference to the element, or
hold a copy of the element in place. The second field however **must**
contain a reference to the next node. In a language such as Java, a node
may be a user-defined object.

```java
class Node {
    int data;
    Node next;
}
```

It would appear that a node contains the next node directly, although
this isn't the case. In this Java source `Node` is a reference type. The
allocation of a new node and pointer logic are all handled implicitly.
This is the equivalent of a C structure containing a pointer to another
structure of the same type.

```c
struct Node {
    int data;
    struct Node * next;
}
```

It is not possible for a C structure to contain a field of its own type
as it would appear in Java, as the structure definition is incomplete at
the point the field is declared. Attempting to declare such a structure
is impossible, it would require an infinite amount of memory.

```c
struct Node {
    int data;
    struct Node next; // invalid
}
```

The same linked list is more rigorously represented as a _structure_
containing a start pointer to a node and subsequent nodes are referenced
by a field of the previous node, as in figure [@fig:llist], at the cost
of revealing more implementation details. This removes the confusion of
what the second cell of each two-cell references in the figure
[@fig:abstract_llist]. The value of each node can also be written more
compactly inside the first two cell.

In short, there are practical reasons to model a whole linked list as a
data type in its own right and there are different stylistic approaches
to illustrating linked lists, evident in figures [@fig:abstract_llist]
and [@fig:llist].

![A more concrete linked list
illustration](../../res/llist.svg){#fig:llist}

In keeping with the inductive type definition, it is unnecessary to
illustrate both nodes and a dedicated list object. It is still possible
to write a linked list implementation where the only user defined type
is a node. It is useful to have a separate linked list type to store
additional metadata about the list such as size or tail pointers in the
case of a _queue_. In either case, such a type can be abstracted away
and it is assumed there is some reference to the first node somewhere in
the program when a linked list is expressed as a simple collection of
nodes.

As an example of a linked list variation, the last node may reference
the first node, the same as the start pointer. Some program may need to
begin performing an operation at some arbitrary point within the list,
other than the first element, in which case this change is helpful. This
is called a _circular_ linked list.

![A circular linked
list](../../res/circular_llist.svg){#fig:circular_llist}

A _queue_ is a linked list with an additional two-cell pointing to the
front and rear nodes.

![A linked list with start and rear
pointer](../../res/queue_llist.svg){#fig:queue_llist}

A node could contain a pointer for the next and previous nodes, called a
_doubly-linked_ or _double linked_ list. If the list additionally
implements the circular property, it can behave as a queue (the rear
pointer is the previous node of the start).

![A circular doubly-linked
list](../../res/double_circular_llist.svg){#fig:double_circular_llist}

# Dynamic Arrays

An array is allocated once with a given size. An array is an ideal
container type, if the data to be stored is of the same form and the
quantity of data is know at compile time. If the maximum size of a data
structure required to store some elements is not known at compile time,
it is still possible to use an array with some additional caveats. An
array capable of growing in this way is known as a _dynamic array_ or
_array list_.

Array size is a type of program invariant, although an array list is
characterised by two variables: the current maximum _capacity_ of the
array and the _size_ or number of elements. Insertion when size is less
than capacity is $O(1)$. Insertion when size is equal to capacity is of
order $O(n)$, the array must be reallocated to make space for more
elements. Figure [@fig:array_list] depicts the growth of a dynamic
array, reallocation is indicated with an arrow.

![Reallocation of an array list](../../res/array_list.svg){#fig:array_list}

Using an array to implement a data structure has the potential to make
insert/remove/access operations faster, although there are drawbacks
with this approach. For any data structure that is allocated at a given
size, such as an array, it is possible the data structure becomes full,
this condition needs to be handled. Array lists can have worse space
complexity if more space than is required is allocated and under certain
conditions the time complexity of an operation may be worse, such as the
reallocation of memory.

