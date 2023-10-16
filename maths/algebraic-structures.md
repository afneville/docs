---
title: Algebraic Structures
author: Alexander Neville
date: 2023-01-03
---

An _algebraic structure_ consists of a non-empty set $A$ (know as the
underlying set, carrier set or domain), a collection of operations on
$A$ and a finite set of axioms for operations on $A$ to satisfy.

# Field

A _field_ is one such algebraic structure for which addition,
subtraction, multiplication and division are defined. A field is a set
$F$ equipped with two binary operations: addition and multiplication.
The result of the addition of two elements $a$ and $b$ is called their
sum, written $a+b$. The result of the multiplication of $a$ and $b$ is
called their product, written $ab$, $a \cdot b$ or $a \times b$. Fields
operations have some axioms.

- Associativity of addition: $a + (b + c) = (a + b) + c$.
- Associativity of multiplication:
  $a \cdot (b \cdot c) = (a \cdot b) \cdot c$.
- Commutativity of addition: $a + b = b + a$.
- Commutativity of multiplication: $a \cdot b = b \cdot a$.
- Additive identity element $0 \in F$: $a + 0 = a$.
- Multiplicative identity element $1 \in F$: $a \cdot 1 = a$.
- Additive inverse of all elements $a$ is $-a$: $a + (-a) = 0$.
- Multiplicative inverse of all non-zero elements $a$ is $a^{-1}$:
  $a \cdot {a^{-1}} = 1$.
- Distributivity of multiplication over addition:
  $a \cdot (b + c) = (a \cdot b) + (a \cdot c)$.

The additive inverse of $a$ is written $-a$. The multiplicative inverse
of $a \neq 0$ is written $a^{-1}$. The _\"inverse operations\"_ of
subtraction $a-b$ and division $a/b$ can be defined in terms of the
inverse element of $a$ and $b$.

$$a-b \stackrel{\text{def}}{=} a + (-b)$$
$$a/b \stackrel{\text{def}}{=} a \cdot (b^{-1})$$

A field is similar to a _commutative ring_ with the addition of the
multiplicative inverse for all non-zero elements of $F$. The rational
numbers $\mathbb{Q}$ and real numbers $\mathbb{R}$ form a field.

# Ring & Commutative Ring

A _ring_ is an algebraic structure similar to a field, but without the
need for multiplication to be commutative and the need for elements to
have a multiplicative inverse. In most cases a ring is said to contain
the multiplicative identity element axiom, the list of ring axioms
being:

- Associativity of addition: $a + (b + c) = (a + b) + c$.
- Associativity of multiplication:
  $a \cdot (b \cdot c) = (a \cdot b) \cdot c$.
- Commutativity of addition: $a + b = b + a$.
- Additive identity element $0 \in F$: $a + 0 = a$.
- Multiplicative identity element $1 \in F$: $a \cdot 1 = a$.
- Additive inverse of all elements $a$ is $-a$: $a + (-a) = 0$.
- Distributivity of multiplication over addition:
  $a \cdot (b + c) = (a \cdot b) + (a \cdot c)$.

In number theory, the binary multiplication operation is often
commutative, in which case the set forms a _commutative ring_. If
$a \cdot b = b \cdot a$, where $a$ and $b$ are elements of a ring $R$,
the ring $R$ is a commutative ring and has the additional commutativity
of multiplication axiom.

- Commutativity of multiplication: $a \cdot b = b \cdot a$.

Division is not defined in a ring in the same way division is defined in
$\mathbb{R}$ or $\mathbb{Q}$. As is the case with a field, the
subtraction operation can be defined using the additive inverse axiom.

$$a-b \stackrel{\text{def}}{=} a + (-b)$$

More often than not, rings are commutative and the word ring is used in
place of commutative ring. The set of integers $\mathbb{Z}$ forms a
commutative ring.

# Commutative Semiring

A commutative semiring is an algebraic structure similar to a
commutative ring, but without the requirement that each element of the
carrier set has an additive inverse. A semiring is commutative if its
multiplication is commutative.

- Associativity of addition: $a + (b + c) = (a + b) + c$.
- Associativity of multiplication:
  $a \cdot (b \cdot c) = (a \cdot b) \cdot c$.
- Commutativity of addition: $a + b = b + a$.
- Commutativity of multiplication: $a \cdot b = b \cdot a$.
- Additive identity element $0 \in F$: $a + 0 = a$.
- Multiplicative identity element $1 \in F$: $a \cdot 1 = a$.
- Distributivity of multiplication over addition:
  $a \cdot (b + c) = (a \cdot b) + (a \cdot c)$.

The set of natural numbers $\mathbb{N}$ forms a commutative semiring.

# Closure

A subset of a set is said to be _closed_ under an operation of the
containing set if the result of the operation on a member of a subset
always produces a member of that subset.

# Modular Arithmetic

Two integers $a$ and $b$ are _congruent modulo_ $n$ if $n>0$ and the
difference of $a$ and $b$ is a multiple of the modulus $n$; there exists
an integer $k$ such that $a - b = kn$.

$$a \equiv b \text{ }(\text{mod } n)$$

In the definition of congruence mod $n$, the brackets means the mod
operation applies to both left and right hand side, it is not just the
modulo of $b$ by divisor $n$. If two numbers are congruent modulo $n$
they have a common remainder $r$ when divided by $n$.

$$a = pn + r$$ $$b = qn + r$$

If $a \equiv x \text{ }(\text{mod } n)$ and
$b \equiv y \text{ }(\text{mod } n)$:

$$a + b \equiv x+y\text{ }(\text{mod } n)$$
$$a - b \equiv x-y\text{ }(\text{mod } n)$$
$$a \times b \equiv x \times y\text{ }(\text{mod } n)$$

## Rings of Modular Arithmetic

Congruence modulo $n$ is an equivalence relation and an integer $a$ has
the equivalence class $\overline{a}_n$, known as the _congruence class_
or _residue class_, all the numbers which have the same remainder when
divided by $n$. The set of all residue classes of the integers modulo
$n$, $\mathbb{Z}_n$ or $\mathbb{Z}/n\mathbb{Z}$ is know as the _least
residue system_ and defined to be:

$$\mathbb{Z}_n = \{a \text{ mod } n \text{ }| \text{ } a \in \mathbb{Z}\} = \{0, \ldots, n-1\}$$

For integers in this set some functions can be defined:

$$a +_{n} b \stackrel{\text{def}}{=} (a+b) \text{ mod } n$$
$$a -_{n} b \stackrel{\text{def}}{=} (a-b) \text{ mod } n$$
$$a \times_{n} b \stackrel{\text{def}}{=} (a \times b) \text{ mod } n$$

$\mathbb{Z}$ is linked to $\mathbb{Z}_n$:

$$(a + b) \text{ mod } n = (a \text{ mod } n) +_{n} (b \text{ mod } n)$$
$$(a - b) \text{ mod } n = (a \text{ mod } n) -_{n} (b \text{ mod } n)$$
$$(a \times b) \text{ mod } n = (a \text{ mod } n) \times_{n} (b \text{ mod } n)$$

## Fields of Modular Arithmetic

An element $a$ of $\mathbb{Z}_n$ has a multiplicative inverse if and
only if $a$ is coprime with $n$. If $n$ is prime then all the elements
of $\mathbb{Z}_n$ are coprime with $n$ and have a multiplicative
inverse. $\mathbb{Z}_n$ forms a field when $n$ is prime.
