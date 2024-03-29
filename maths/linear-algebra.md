---
author: Alexander Neville
date: 2023-01-05
title: Linear Algebra
---

# Gaussian Elimination

Gaussian elimination is a technique for solving a system of linear
equations with $n$ equations and $n$ variables, using the _matrix of
coefficients_ and the elementary row operations:

1.  Rearrange two rows.
2.  Multiply a row by a scalar.
3.  Add two rows.

_Row echelon form_ has the first non-zero element of each row in the
main diagonal and 0 coefficients below the main diagonal. This form
gives one coefficient directly and makes back substitution easy.

$$\left[\begin{array}{ccc|c}1&\ast&\ast&\ast \\ 0&1&\ast&\ast \\ 0&0&1&\ast \end{array} \right ]$$

Sometimes _reduced row echelon form_ is preferred for reading
coefficients more easily.

$$\left[\begin{array}{ccc|c}1&0&0&\ast \\ 0&1&0&\ast \\ 0&0&1&\ast \end{array} \right ]$$

# Vector Spaces

A _vector space_ over a field $F$ is a set $V$ equipped with two binary
operations: vector addition and scalar multiplication. Any element of
$V$ is a vector, regardless of its form (assuming the conditions for a
vector space are upheld). Vector addition assigns two vectors $\vec{v}$
and $\vec{w}$ in $V$ to a third vector $\vec{v} + \vec{w}$ in $V$,
otherwise written $\vec{v} \oplus\vec{w}$. Scalar multiplication assigns
a vector $\vec{v} \in V$ and a scalar $s \in F$ to a vector $s \vec{v}$.
Sometimes $\boldsymbol{v}$, $\boldsymbol{w}$ notation is used in place
of $\vec{v}$ and $\vec{w}$ to distinguish vectors from scalars. Vector
spaces uphold some axioms.

- Associativity of vector addition:
  $\vec{u} \oplus (\vec{v} \oplus \vec{w}) = (\vec{u} \oplus \vec{v}) \oplus \vec{w}$.
- Commutativity of vector addition:
  $\vec{u} \oplus \vec{v} = \vec{v} \oplus \vec{u}$.
- Identity element of vector addition $\boldsymbol{0} \in V$:
  $\vec{v} \oplus \boldsymbol{0} = \vec{v}$.
- Additive inverse of vector addition $-\vec{v} \in V$:
  $\vec{v} \oplus -(\vec{v}) = 0$.
- Associativity of scalar & vector multiplication:
  $r(s\vec{v}) = (rs)\vec{v}$
- Identity element of scalar multiplication $1 \in F$:
  $1\vec{v} = \vec{v}$
- Distributivity of scalar multiplication:
  $(r+s)\vec{v} = r\vec{v} \oplus s\vec{v}$
- Distributivity of scalar multiplication:
  $s(\vec{v} + \vec{w}) = s\vec{v} \oplus s\vec{w}$

Some derived statements about vector spaces:

- $\vec{v} - \vec{w} = \vec{v} \oplus (-\vec{w})$
- $0\vec{v} = \boldsymbol{0}$
- $s\boldsymbol{0} = \boldsymbol{0}$
- $(-1)\vec{v} = -\vec{v}$
- $s\vec{v} = \boldsymbol 0 \implies s = 0 \lor \vec{v} = \boldsymbol{0}$

## Examples

Every field is a vector space over itself. Fields are closed under $+$
and $\times$, thus $\oplus$ can be defined as $+$ and scalar
multiplication is simply $\times$. The set of _n_-tuples of rational
numbers $\mathbb{Q}$ is a vector space over $\mathbb{Q}$.

$$\mathbb{Q}^n \stackrel{\text{def}}{=} \{(a_1, \ldots, a_n) \text{ } | \text{ } a_1, \ldots a_n \in \mathbb{Q}\}$$

An _n_-tuple is sometimes written vertically if it is an element of a
vector space.

$$(a_1, \ldots, a_n) = \begin{pmatrix} a_1\\ \vdots \\a_n \end{pmatrix}$$

For any _n_-tuple, vector addition and scalar multiplication are
defined:

$$\begin{pmatrix} a_1\\ \vdots \\a_n \end{pmatrix} \oplus \begin{pmatrix} b_1\\ \vdots \\b_n \end{pmatrix} \stackrel{\text{def}}{=} \begin{pmatrix} a_1 & + & b_1\\ \vdots & + & \vdots \\a_n & + & b_n \end{pmatrix}$$

$$s\begin{pmatrix} b_1\\ \vdots \\b_n \end{pmatrix} \stackrel{\text{def}}{=} \begin{pmatrix} s \times a_1\\ \vdots \\ s \times a_n \end{pmatrix}$$

## Span

The span of two vectors $\vec{v}$ and $\vec{w}$ is the set of all
possible linear combinations of $\vec{v}$ and $\vec{w}$.

$$\text{Span}(\vec{v}, \vec{w})\stackrel{\text{def}}{=} \{r\vec{v} \oplus s\vec{w} \text{ } | \text{ } r,s \in F\}$$

Any set of vectors has a span:

$$\text{Span}(\vec{v}_1, \ldots, \vec{v}_n)\stackrel{\text{def}}{=} \{r_1\vec{v}_1 \oplus \ldots \oplus r_n\vec{v}_n \text{ } | \text{ } r_1, \ldots ,r_n \in F\}$$

A vector belongs to the span of a set of vectors
$\{\vec{v}_1, \ldots, \vec{v}_n\}$, if it can be written as a linear
combination of the set of vectors. If the span of a set of vectors is
all of $V$, every element of $V$ is a linear combination of
$\{\vec{v}_1, \ldots, \vec{v}_n\}$, it is a _spanning set_.

## Linear Independence

A set of vectors $\{\vec{v}_1, \ldots, \vec{v}_n\}$ is _linearly
independent_ if
$r_1\vec{v}_1 \oplus \ldots \oplus r_n\vec{v}_n = \vec{0} \implies r_1 = \ldots = r_n = 0$.
The only way to obtain the zero vector $\vec{0}$ is by taking each
scalar to be $0$.

## Basis

A set of vectors $\vec{v}_1, \ldots, \vec{v}_n \in V$ forms a _basis_ if
$\text{Span}(\vec{v}_1, \ldots, \vec{v}_n) = V$ and
$\{\vec{v}_1, \ldots, \vec{v}_n\}$ is linearly independent. The inner
product of two _n_-tuple vectors is defined to be:

$$\vec{v} \cdot \vec{w} \stackrel{\text{def}}{=} (v_1 \times w_1) + \ldots + (v_n \times w_n)$$

A basis set of vectors $B$ is _orthogonal_ if the inner product of all
elements of $B$ with each other is $0$.

# Matrices

A matrix is a two-dimensional array of values from a field. Matrices can
be read row-wise or column-wise. The size of a matrix is $m \times n$
where $m$ is the number of rows and $n$ is the number of columns. The
notation $a_{ij}$ means the element in row $i$ and column $j$.

$$A_{2\times 3 } = \begin{bmatrix} a_{11} & a_{12} & a_{13} \\ a_{21} & a_{22} & a_{23}\end{bmatrix}$$

## Matrix Multiplication

Matrix multiplication is defined for two matrices $A_{m \times n}$ and
$B_{n \times p}$, the number of rows in $B$ must equal the number of
columns in $A$. Elements of $A$ are given by $a_{ij}$ and elements of
$B$ are given by $b_{jk}$ The result is a $m \times p$ matrix, whose
element in row $i$ and column $k$ is the inner product of row $i$ of $A$
and column $k$ of $b$.

$$c_{ik} = \sum_{j=1}^{n} a_{ij} \times b_{jk} $$

Matrix multiplication is associative but not commutative.

## Matrix Inversion

The inverse of a matrix $A$ is $A^{-1}$, such that $AA^{-1} = I$, where
$I$ is the identity matrix. Not all matrices have an inverse. The
identity matrix is a square matrix and has 1 in the main diagonal and 0
everywhere else. $I_2, I_3, I_4$:

$$\left [ \begin{array}{cc} 1&0 \\ 0&1\end{array} \right ] \left [ \begin{array}{ccc} 1&0&0 \\ 0&1&0 \\ 0&0&1\end{array} \right ] \left [ \begin{array}{cccc} 1&0&0&0 \\ 0&1&0&0 \\ 0&0&1&0 \\ 0&0&0&1\end{array} \right]$$

To calculate the inverse for a matrix, write $[A|I]$ and perform
elementary row operations to write $A$ as the identity matrix, which
will arrange $I$ into $A^{-1}$.

$$\left[\begin{array}{ccc|ccc} \ast&\ast&\ast&1&0&0\\ \ast&\ast&\ast&0&1&0\\ \ast&\ast&\ast&0&0&1\\\end{array} \right ] \rightarrow \left[\begin{array}{ccc|ccc} 1&0&0&\ast&\ast&\ast\\ 0&1&0&\ast&\ast&\ast\\ 0&0&1&\ast&\ast&\ast\\\end{array} \right ]$$
