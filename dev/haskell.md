---
title: Haskell Features
author: Alexander Neville
date: 2023-10-20
---

An expression is any piece of code which returns or _yields_ a value.
All computation in Haskell and more generally any functional language is
conducted by evaluating these expressions.

# Functions

In Haskell, as in mathematics, a function is a mapping from a number of
arguments to a single result. Functions are defined with equations,
featuring the name of a function, its parameters and a body that
computes the result in terms of the arguments passed.

Functions are left associative. Function application has higher
precedence than infix operators. Functions are usually prefix and space
is used for function application. Make a prefix function infix surround
it with backticks, to make an infix function prefix, surround it with
brackets.

```hs
2 `max` 3
(*) 2 3
```

## Guards

- `|` is read as _such that_.
- `otherwise` always evaluates to `True`.

```hs
abs' :: (Ord a, Num a) => a -> a
abs' n | n >= 0 = n
       | otherwise = -n

classification x =
    if x > 100 then "No Classification"
    else if x >= 70 then "First"
    else if x >= 60 then "Upper Second"
    else if x >= 50 then "Lower Second"
    else if x >= 40 then "Third"
    else "No Classification"

classification' x
    | x > 100 = "No Classification"
    | x >= 70 = "First"
    | x >= 60 = "Upper Second"
    | x >= 50 = "Lower Second"
    | x >= 40 = "Third"
    | otherwise =  "No Classification"
```

## Pattern Matching

```hs
land :: Bool -> (Bool -> Bool)
land True True   = True
land True False  = False
land False True  = False
land False False = False
```

```hs
land' :: Bool -> (Bool -> Bool)
land' True True = True
land' _    _    = False
```

The list construction operator can be used in pattern matching.
Surrounding the pattern in brackets is required as the precedence of `:`
is lower than function application.

```hs
head' :: [a] -> a
head' (x:_) = x

tail' :: [a] -> [a]
tail' (_:xs) = xs
```

## Lambda Expressions

Functions may be defined with equations or nameless _lambda
expressions_. Lambda expressions contain a pattern for each of the
arguments and a body to compute the return value. Lambda Expressions may
be bound to a symbol.

```hs
(\x -> x + x) 2
double = (\x -> x + x)
double 2
```

## Higher-order Functions

# Types & Typeclasses

- A type expression denotes type values or _types_.
- A _type_ is a collection of related values.
- Every expression has a type.
- `::` is read as _has the type of_.
- Type inference is always performed before evaluation of an expression.
- Haskell programs are type safe. Type Errors cannot occur during
  evaluation.
- Haskell has a static type system, but the compiler is able to infer
  types.
- Value identifiers must begin with a lowercase letter, while type
  identifiers must be capitalised. The exception is _type variables_.
- `->` reads as _maps_ and associates to the right
- Fewer than the full number of arguments can be passed to a curried
  function to return a _partially applied function_.
- Function application usually associates to the left.

```hs
multiplythree :: Int -> (Int -> (Int -> Int))
multiplythree x y z = x * y * z
```

```hs
multiplytwo = (multiplythree 1) :: Int -> (Int -> Int)
```

```hs
multiplythree' :: Int -> (Int -> (Int -> Int))
multiplythree' =  \x  -> (\y  -> (\z  -> x * y * z))
```

- A class is is a collection of types that support certain operations,
  known as methods.

The `type` keyword introduces a new name for an existing type.

```hs
type Coord = (Int,Int)
```

Type definitions can not by cyclic or recursive, the following is not
legal:

```hs
type Tree = (Int,[Tree])
```

Type definitions can be parameterised

```hs
type Triple a = (a,a,a)
a = (1,2,3) :: Triple Int
type Pair a b = (a,b)
b = ('a',1) :: Pair Char Int
```

A new type, distinct from any other, can be declared using the `data`
keyword. `|` is read as _or_. The type is defined by specifying its
values. The names of these values must, however, be unique. The values
are described as _constructors_.

```hs
data CardinalPoint = North | East | South | West
a = North :: CardinalPoint
```

Constructors may have parameters, in which case they might me called
_constructor functions_.

```hs
data Shape = Circle Float | Rectangle Float Float
```

Constructor functions differ from _normal_ functions because they are
not defined with equations. They are fully evaluated.
`Rectangle 2.0 3.0` is a value just as `1.0` is.

The `data` keyword does allow recursive type definitions, with which the
standard pattern matching and recursive techniques can be employed.

```hs
data List a = Empty | Cell a (List a)

length' :: List a -> Integer
length' Empty = 0
length' (Cell _ l) = 1 + length' l

toArray :: List a -> [a]
toArray Empty = []
toArray (Cell x l) = x:(toArray l)
```

```language-plaintext
> :t Rectangle
Rectangle :: Float -> Float -> Shape
> :t Circle
Circle :: Float -> Shape
```

## Built-in Types

# Lists & Tuples

- `[]` is the empty list.
- `:` is the _cons_ operator that returns a new list by prepending a new
  element to the start of a list.
- `:` associates to the right.

Tuples of arity 1 are not allowed as the syntax would conflict with the
use of brackets to make the order of evaluation explicit.

## Comprehensions

Haskell boasts a comprehension notation to construct specific lists,
similar to mathematical comprehensions that construct bespoke sets from
more general ones. `[f x | x <- xs]` is read as _the list of all_ `f x`
_such that_ `x` _is drawn from_ `xs`. In this example, the expression
`x <- xs` is called a _generator_. A comprehension may have more than
one generator, each separated by a comma. Generators appearing to the
right cycle more frequently than those to the left. Consider the
example:

```hs
[(x,y) | x <- ['a', 'b'], y <- [1,2]] == [('a',1),('a',2),('b',1),('b',2)]
```
