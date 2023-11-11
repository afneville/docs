---
title: Functional Programming
author: Alexander Neville
date: 2023-10-17
---

Functional programming is a _ programming paradigm_. A paradigm is a
broad characterisation of a language and its features. The functional
programming paradigm falls into the category of _declarative_ languages,
in which the program is more a description of computation than a
sequential set of instructions (which would be described as imperative).
Functional programming languages treat mathematically pure first-class
functions as the fundamental method of computation.

Many languages which are not inherently functional draw inspiration from
functional programming. It is common to see functions treated as first
class objects.

# First-class Functions

Functions in functional programming are considered _first-class_ -
functions can be passed to and returned from other functions and bound
to variables, just as any other data type.

# Purely Functional Programming

Purely functional programming is a subset of the functional programming
paradigm, in which functions are _pure_ or
[mathematically](/maths/relations.md#functions) deterministic. Functions
in many imperative languages are more accurately described as
procedures, because they do not satisfy the definition of a mathematical
function. Pure functions are guaranteed to return the same value across
repeated invocations with consistent arguments. The computation of the
function is not affected by any mutable program state. Similarly, a pure
function does not manipulate any state or cause side-effects.

# Higher-order Functions

A higher-order function simply takes a function as an argument or
returns a function. Support for higher-order functions implies that
functions are first-class objects.

- map
- fold
- filter

# Currying

A _n_-ary function can take all of its arguments at once, packed into a
tuple. Alternative a function of high arity can be translated into a
sequence of unary functions, a technique known as _currying_. If $( )$
brackets are used to denote function application, the function $f$
taking three arguments would be transformed into a unary function $g$
returning a new unary function that accepts one argument and returning
another unary function which finally returns the result of the entire
computation of $f$.

$$f(x,y,z) \implies g(x)(y)(z)$$

Currying is a mathematical technique and functions with multiple
arguments in _lambda calculus_ are usually represented in their curried
form. Some programming languages like [Haskell](./haskell.md) enforce
currying - all Haskell functions accept just one argument (though this
argument could be a tuple containing multiple values).

# Partial Application

It is very useful that a language curries functions with multiple
arguments as it facilitates _partial application_ - the same can be
achieved with the use of _closures_.

In the case of a curried function, simply passing fewer than the full
number of arguments to a function will return a partially applied
function with bound values and lower arity than the initial function.
Haskell supports curried functions and function application is written
with spaces, thus the following example demonstrates the creation of an
increment function by partially applying `1` to the binary `+` function.

```language-plaintext
> g = (+) 1
> g 2
3
```

In non-functional languages, partial application is still achievable by
using closures. A closure is a scoped binding of a value to a function
which may be used outside of the scope in which the captured variables
exist.

```python
def f(x):
    def g(y):
        return x + y
    return g

def h(x):
    return lambda y: x + y
```

The closures returned by `f` and `g` can be bound to symbols in a
different scope to the captured variables.

```language-plaintext
>>> g = f(2)
>>> i = h(4)
>>> g(2)
4
>>> i(2)
6
```

# Referential Transparency

Generally, variables in functional programming are immutable. The value
of a variable cannot be modified once defined. Consequently, every
occurrence of a variable in a functional program can be replaced with
its value - a situation referred to as referential transparency.
