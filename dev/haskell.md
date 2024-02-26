---
title: Haskell Programming
author: Alexander Neville
date: 2023-10-20
image-meta: haskell-logo.png
image-meta-credit: https://commons.wikimedia.org/wiki/File:Haskell-Logo.svg
---

Haskell is a purely functional programming language. An expression in
Haskell is a piece of code which returns or _yields_ a value. All
computation in Haskell is conducted by evaluating these expressions.
Every expression in Haskell has an associated type. Whilst _strict_, the
Haskell compiler is able to infer the type of a value or expression and
consequently Haskell code need not be explicitly typed.

# Functions & Syntactic Features

Function application is typically left associative and introduced by a
space. Prefix functions are conventional and take precedence over infix
operators. A prefix function can be made infix by surrounding it with
backticks. Conversely, an infix function can be made prefix, by
surrounding it with brackets.

```hs
2 `max` 3
(*) 2 3
```

The mathematical operators are not special syntactic elements, they are
simply functions. Operators have their own fixity, precedence and
associativity rules. Precedence can be made explicit with brackets. The
following expression evaluates to 14.

```hs
2 * (3+4)
```

## Composition

The `$` symbol is the infix function application operator. It is right
associative and has lower precedence than function application with
space. The following two examples both yield 27.

```hs
(*) 3 (4 + 5)
(*) 3 $ 4 + 5
```

## Function Application

The `.` symbol performs function composition - the application of one
function to the result of another. The following two examples both
yield 20.

```hs
(+4) ((*4) 4)
((+4).(*4)) 4
```

## Equations

In Haskell, as in mathematics, a function is a mapping from a number of
arguments to a single result. Functions are defined with equations,
featuring the name of a function, its parameters and a body that
computes the result in terms of the arguments passed.

```hs
add x y = x + y
```

## Guards

A guarded function evaluates a boolean expression to determine the
equation to use.

- `|` is read as _such that_.
- `otherwise` always evaluates to `True`.

```hs
abs' :: (Ord a, Num a) => a -> a
abs' n | n >= 0 = n
       | otherwise = -n

```

Here is an example of the same function written first with a number of
conditional expressions and then guards. Note that each conditional must
include an expression to evaluate if the condition is not true, hence
every else in this function is associated with the most recent if
statement.

```hs
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

## Where, Let & Case

A _where clause_ can be used to bind the result of an expression to a
variable in the surrounding syntactic construct, spanning across a set
of guards in a function equation. This example additionally demonstrates
the case statement which permits pattern matching within an expression.

```hs
grade raw total
    | pc >= 70 = 'A'
    | pc >= 60 = 'B'
    | pc >= 50 = 'C'
    | otherwise =  'F'
    where pc = topc $ tofrac raw total
             where tofrac a b = case (a, b) of
                                            (_, 0) -> 0
                                            (a, b) -> if a < b
                                                      then a / b
                                                      else 0
                   topc 0 = 0
                   topc frac = frac * 100
```

A _let clause_ can be used to the same effect, though its scope is
limited to an expression.

```hs
let x = 5 in x + 10 * x
```

## Pattern Matching

A function in Haskell may be defined with a series of equations. A
particular equation is used as the function implementation if the
arguments passed match the input pattern.

```hs
land :: Bool -> (Bool -> Bool)
land True True   = True
land True False  = False
land False True  = False
land False False = False
```

A pattern may do some introspection on a value and bind arguments to
variables in the corresponding equation. This is sometimes called
_deconstruction_. There is no limit to how thoroughly a pattern may
deconstruct its arguments. The three constituents of a triple can be
extracted with a pattern, for example.

```hs
third (a,b,c) = c
```

The `_` character can be used as a _wild card_ in a pattern. It will
match any value and it may additionally be repeated, unlike a named
variable, though it cannot be subsequently referenced in the equation.
The logical and function from above can be written more concisely with
the use of the wild card.

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

The `@` is a special syntactic feature used in an _as-pattern_. If a
pattern to the right of the symbol matches, the value of the whole
expression will be bound to the identifier to its left.

```hs
firstlast [] = []
firstlast xs@(_:_) = [(head xs), (last xs)]
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

# Lists & Tuples

- `[]` is the empty list.
- `:` is the _cons_ operator that returns a new list by prepending a new
  element to the start of a list. Associates to the right.

Tuples of arity 1 are not allowed as the syntax would conflict with the
use of brackets to make the order of evaluation explicit.

## Comprehensions

Haskell boasts a comprehension notation to construct specific lists,
similar to mathematical comprehensions that construct bespoke sets from
more general ones. `[f x | x <- xs]` is read as _the list of all_ `f x`
_such that_ `x` _is drawn from_ `xs`. In this example, the expression
`x <- xs` is called a _generator_. A comprehension may have more than
one generator, each separated by a comma. Generators appearing to the
right cycle more frequently than those to the left.

```hs
[(x,y) | x <- ['a', 'b'], y <- [1,2]] == [('a',1),('a',2),('b',1),('b',2)]
```

# Types

A _type expression_ denotes type values or _types_. A type is a
collection of related values. Every expression has a type. Value
identifiers must begin with a lowercase letter, while type identifiers
must be capitalised, except in the case of _type variables_.

- `::` is read as _has the type of_ and introduces a type expression. It
  may additionally be used for explicit type coercion.
- `->` reads as _maps_ and associates to the right, while function
  application usually associates to the left.

Type inference is always performed before evaluation of an expression.
Consequently, Haskell programs are type safe; type errors cannot occur
during evaluation. Haskell has a static type system - type inference and
checking occurs at compile time. There are compelling reasons to
annotate expressions and functions explicitly, rather than let the
compiler infer an expression's type. A function can be explicitly typed
as so:

```hs
multiplythree :: Int -> (Int -> (Int -> Int))
multiplythree x y z = x * y * z
```

The same function can be written using lambda expressions, making the
equation resemble the type expression more closely.

```hs
multiplythree' :: Int -> (Int -> (Int -> Int))
multiplythree' =  \x  -> (\y  -> (\z  -> x * y * z))
```

The following type expression illustrates how types are consistent and
predictable under partial application.

```hs
multiplytwo = (multiplythree 1) :: Int -> (Int -> Int)
```

## Type Synonyms

The `type` keyword introduces a new name for an existing type.

```hs
type Coord = (Int,Int)
```

Type definitions can not by cyclic or recursive, the following is not
legal:

```hs
type Tree = (Int,[Tree])
```

Type definitions can be parameterised.

```hs
type Triple a = (a,a,a)
a = (1,2,3) :: Triple Int
type Pair a b = (a,b)
b = ('a',1) :: Pair Char Int
```

## Creating Data Types

A new type, distinct from any other, can be declared using the `data`
keyword. `|` is read as _or_. The type is defined by specifying its
values. The names of these values must, however, be unique. The values
are described as _data constructors_. The name of the new type would be
considered a _type constructor_.

```hs
data CardinalPoint = North | East | South | West
a = North :: CardinalPoint
```

A data type with multiple values is called an _algebraic data type_, or
sometimes an _enumerated type_. Value constructors may be parametrised
in which case they might me called _constructor functions_ or _value
constructors_. Constructors must have unique names.

```hs
data Shape = Circle Float | Rectangle Float Float
```

The types that follow each value constructor are considered _components_
of the type. The constructors `Cicle` and `Float` return a value of type
`Shape` when passed components of the correct type.

```hs
rectangle = Rectangle :: Float -> Float -> Shape
circle = Circle :: Float -> Shape
```

Constructor functions differ from _normal_ functions because they are
not defined with equations. They are fully evaluated.
`Rectangle 2.0 3.0` is a value just as `1.0` is. A value constructor may
safely be assigned the same identifier as a data constructor, as one is
used only in type expressions, whereas the other is used only in source
code.

## Recursive Type Definitions

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

## Typeclasses

A class is is a collection of types that support certain operations,
known as methods. A new class can be defined and a type can be made an
instance of a class.

```hs
data Direction = Left' | Right'

class Eq' a where
    isEqual :: a -> a -> Bool
    isNotEqual :: a -> a -> Bool
    isNotEqual x y  = not (isEqual x y)

instance Eq' Direction where
    isEqual Left' Left' = True
    isEqual Right' Right' = True
    isEqual _ _ = False
```

## Creating New Type Identities

The `type` keyword creates a type synonym, not a distinct type, while
the `data` keyword can be used to create a bespoken algebraic type. The
`newtype` keyword is said to create a new identity for an existing type.
The new type is not interchangeable with the original, unlike a type
synonym. A `newtype` declaration may have only one type value
constructor with precisely one field. This type can be added to
instances independently of the original type.

```hs
newtype Int' = N Int

instance Show Int' where
      show (N x) = show x

instance Eq Int' where
      (==) (N x) (N y) = x == y
```

# Kinds

Expressions are annotated with types, similarly types are annotated with
kinds. The kinds of a type expression can be inspected in _ghci_ with
`:k`. The `*` symbol represents a concrete type. `Int` and `Maybe Int`
are concrete types, but `Maybe` is a type constructor which returns a
concrete type when passed a type, as an example.

```text
ghci> :k Int
Int :: *
ghci>
ghci> :k Maybe
Maybe :: * -> *
ghci> :k Maybe Int
Maybe Int :: *
```

# Monads

Polymorphism is one very good way of achieving generality in Haskell.
Abstracting common design patterns into a typeclass which a type can
implement can also be of benefit. _Functors_ capture the concept of
mapping a function over constituent values of a type, _applicatives_
make function application more expressive over types and _monads_
introduce the possibility of effectful programming.

## Functors

A _functor_ is an eponymous member of the functor typeclass. A functor
is sometimes referred to as a _container_, but _computational context_
is more accurate. A type constructor must have one type parameter to be
made a functor. Type constructors with higher arity may be partially
applied, of course.

A functor captures the concept of mapping a function over a parametrised
type. Instances of `Functor` must implement the `fmap` function, which
has the type `(a -> b) -> f a -> f b`.

```hs
data Tree a = Empty | Tree (Tree a) a (Tree a)
        deriving (Show)

instance Functor Tree where
    fmap f Empty = Empty
    fmap f (Tree l v r) = Tree (fmap f l) (f v) (fmap f r)
```

```hs
fmap (+1) $ Tree (Tree (Tree Empty 1 Empty) 2 Empty) 3 (Empty)
```

The `fmap` function has an infix notation: `<$>`.

## Applicative

Functors have the obvious limitation of supporting only unary functions.
It is possible apply a function expecting more than one argument over
multiple structures by applying each partially applied function
manually, but this is not ideal.

```hs
fmap (\f -> f 2) (fmap (+) [1..5])
```

Members of the applicative typeclass support two functions: `pure` and
the infix `<*>` operator. The latter has the type
`f (a -> b) -> f a -> f b` - it accepts functions wrapped in a context
alongside arguments wrapped in the same context. The role of `pure` is
to lift a value into a default minimal context, having the type
`a -> f a`. Here are some examples:

```hs
pure (+) <*> Nothing <*> Just 4   -- Nothing
pure (+) <*> Just 3 <*> Just 4    -- Just 7
Just (+) <*> Just 3 <*> Just 4    -- Just 7
pure (+) <*> [3] <*> [4]          -- [7]
pure (+) <*> [3] <*> [4,5]        -- [7,8]
[(+)] <*> [1,2,3] <*> [4,5,6]     -- [5,6,7,6,7,8,7,8,9]
pure (*) <*> [1,2,3] <*> [2]      -- [2,4,6]
(*) <$> [1,2,3] <*> [2]           -- [2,4,6]
```

Mapping a function over a functor yields a functor containing partially
applied functions - this can be used in an applicative way.

## Monad

The applicative type class allows a pure function to be applied to
potentially effectful (contextual) arguments, but falters when the
function itself is similarly effectful (e.g. a function of type
`a -> f b` is not supported by the applicative typeclass).

The monad typeclass is defined by two functions: `return` and the bind
operator `>>=`. They have the types:

```hs
return :: a -> m a
(>>=)  :: m a -> (a -> m b) -> m b
```

The `return` function serves the same purpose as `pure` - to insert a
value into a minimal context. The _bind_ operator takes the underlying
value of the monad and applies the function argument to it, which should
put the value back into the context. `>>=` can be said to pass a
_non-monadic_ value to a function within a monad.
