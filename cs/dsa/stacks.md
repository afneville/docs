---
title: Stacks
author: Alexander Neville
date: 2023-01-11
---

A stack is an abstract data type organises data in _First-In-Last-Out
(FILO)_ or _Last-In-First-Out (LIFO)_ manner. The most recently inserted
item is the first to be removed from a stack. A stack can be defined
inductively with the constructors `emptystack` and `push`, the
conditional `isemptystack` and the selectors `top` and `pop`.

- `emptystack() -> S`
- `push(E,S) -> S`
- `isemptystack(S) -> T|F`
- `top(S) -> E`
- `pop(S) -> S`

In this stack definition, which does not mutate the state of one stack,
instead creating and returning new stacks as required, `top` returns the
first element of a stack and `pop` returns the remainder of stack,
without the first element. For most practical purposes, a single stack
is used and changed destructively, in which case `push` and `pop` have
different definitions.

- `push(E,S)`
- `pop(S) -> E`

This version of `pop` removes and returns the first element of a stack.
The state of the original stack is changed to reflect the result of the
operation. There is no need to create and return a new stack.

# Implementation

A stack is very easily implemented with a singly linked list. Items are
inserted and removed from the front. In figure [@fig:stack_push_pop] the
integers 7, 2 and 9 are pushed onto the stack. Items are retrieved in
the reverse order of their insertion. The integers pushed onto the stack
are popped from the front and returned in the order 9, 2, 7. In this
example, the whole process mutates a single stack, which is explicitly
depicted in the figure.

![Stack operations, push and
pop](../../res/stack_push_pop.svg){#fig:stack_push_pop}

It is also possible to implement a stack as an array. Items are added
and removed from the rear. The rear position is calculated from the size
of the stack (the number of element in the stack). Accessing any element
in an array can be done in constant time. The stack implementation also
maintains the maximum stack size (size of the underlying array). If the
size of the stack is the allocated size of the array, the stack is
considered _full_. Pushing and further elements onto the stack will
result in a state known as _stack overflow_. A dynamic array stack
implementation can be used to avoid this condition.

![Stack implemented as an
array](../../res/stack_array.svg){#fig:stack_array}

