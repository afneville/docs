---
title: Probability Theory
author: Alexander Neville
date: 2024-02-17
---

The _probability_ of an event is a measure of how likely it is to occur,
expressed as a number in the range $[0,1]$ with $1$ representing
absolute certainty and $0$ representing impossibility.

# Interpretations

The nature of probability is subject to interpretation. Common schools
of thought include _frequentism_, regarding the probability of an event
as the frequency of its occurrence over repeated trials, and
_subjectivisim_, in the view of which probability is a measure of the
_credence_ or conviction with which one believes that a certain outcome
will occur.

# Experiments

In probability theory, an _experiment_ is a repeatable procedure with a
set of _outcomes_. An experiment with more than one outcome is known as
a _random experiment_, as opposed to a _deterministic experiment_, which
has a single outcome. The occurrence of an outcome could entail the
occurrence of any number of _events_, which are sets of outcomes.

## Outcomes

An outcome of a random experiment is a possible result of that
experiment. Each outcome of an experiment is unique and in the case of
random experiments any two outcomes are mutually exclusive.

## Events

An event is a set of outcomes. An event containing a single outcome is
considered an _elementary event_, while an event containing more than
one outcome is called a _compound event_.

## Trials

Many random experiments are, or at least can be, repeated. An experiment
repeated $n$ times is an experiment in its own right and his its own set
of outcomes. For example, two successive coin tosses form a random
experiment with the outcomes
$\{(\text{h},\text{h}),(\text{h},\text{t}),(\text{t},\text{h}),(\text{t},\text{t})\}$,
where $h$ and $t$ represent the coin showing _heads_ and _tails_
respectively. The original experiment, a single coin toss, is often
called a _trial_.

# Probability Space

An experiment is formally modelled with a mathematical construct known
as a _probability space_ or _probability triple_,
$(\Omega, \mathcal{F},\mathrm{P})$, where:

- $\Omega$ is the _sample space_.
- $\mathcal{F}$ is the _event space_.
- $\mathrm{P}$ is a _probability function_ or _probability measure_.

## Sample Space

The sample space $\Omega$ is the set of all possible outcomes. This is a
non-empty set in which a given outcome may appear only once. The
elements of a sample space are mutually exclusive and _collectively
exhaustive_, some outcome belonging to the sample space will occur on
every trial.

## Event Space

The event space $\mathcal{F}$ is a set of events. Each event being a
subset of the event space, the event space is a subset of the powerset
of the sample space $\mathcal{F} \subseteq \mathcal{P}(\Omega)$. In the
case of finite sample spaces, it is conventional to model every subset
of the sample space as an event. Certain events like the empty set
$\emptyset$ containing no outcomes are assigned a probability of $0$,
due to the collect exhaustiveness of the sample space.

## Probability Function

The probability function $\mathrm{P}$ is a mapping from every element of
the event space to a real value,
$\mathrm{P}: \mathcal{F} \rightarrow [0,1]$. The probability function
must assign probabilities to each event summing to $1$.

# Random Variable

A _random variable_ is a mapping from an element of the sample space to
a quantity in a _measurable space_, usually a real interval. A random
variable is often defined $X:\Omega
\rightarrow E$, where $\Omega$
belongs to a probability space $(\Omega, \mathcal{F},P)$. Random
variables are conventionally denoted with capital letters.

## Observations

The realisation of a random variable is called an _observation_. As a
random variable is a function over the sample space, an observation is
an element of the image of the random variable $x = X(\omega)$, where
$\omega \in \Omega$. Observations are conventionally denoted with
lowercase letters.

If $S$ is subset of the measurable set $S \subseteq E$ and a random
variable $X$ is defined over a probability space, then the probability
that a observation of $X$ is in the set $S$ can be calculated with the
probability measure $\mathrm{P}$, given that an event
$\{\omega \in \Omega | X(\omega) = x\}$ is a set of outcomes taken from
the pre-image of $X$ for an observation $x$ of the codomain.

$$\mathrm{P}(X \in S) = \mathrm{P}(\{\omega \in \Omega | X(\omega) \in S\})$$

# Probability Distribution

A _probability distribution_ is, like a probability measure, a function
mapping outcomes of an event to a probability. The term probability
distribution is preferred over measure when the domain is either a
random variable or measurable events, I surmised from
[this](https://math.stackexchange.com/questions/1073744/distinguishing-probability-measure-function-and-distribution)
exchange.

The probability distribution of a random variable is defined by a
_probability mass function_ in the case of countably infinite values and
a _probability density function_ in the case of uncountably many values.

## Probability Mass Function

The _probability mass function_ is a function that gives the probability
of a discrete random variable being precisely equal to some observation.

$$p_{X}(x) = p(x) = \mathrm{P}(X = x)$$

## Probability Density Function

The _probability density function_ is a function that is integrated
between bounds to specify the probability that an uncountably infinite
random variable has a value in that range.

$$\mathrm{P}(a \le X \le b) = \int_a^bf_X(x)dx$$

A distribution is said to _admit_ a density function and different
distributions will define the density function $f_X(x)$ differently.

## Cumulative Distribution Function

The _cumulative distribution function_ of a random variable is the
probability that $X$ takes a value less than or equal to $x$.

$$F_{X}(x) = \mathrm{P}(X \le x)$$

# Conditional Probability

The conditional probability of an event $A$ is its probability in the
knowledge that another event $B$ has occurred.

$$\mathrm{P}(A | B) = \dfrac{\mathrm{P}(A \cap B)}{\mathrm{P}(B)}$$

# Independence

Two events are _independent_, written $A \perp B$, if the occurrence of
one does not change the probability distribution of the other. For this
condition to be satisfied, the _joint probability_ or _intersection_ of
the two events must equal the product of their probabilities.

$$A \perp B \iff \mathrm{P}(A \cap B) = \mathrm{P}(A)\mathrm{P}(B)$$

The intersection of both events (the outcomes they have in common) must
be non-empty, $A \cap B \neq \emptyset$, which indicates _mutual
exclusion_. Additionally, the conditional probability of each event
given the other is equal to the probability of the event itself.

$$\mathrm{P}(A \cap B) = \mathrm{P}(A)\mathrm{P}(B) \iff \mathrm{P}(A | B) = \dfrac{\mathrm{P}(A \cap B)}{\mathrm{P}(B)} = \mathrm{P}(A)$$

$$\mathrm{P}(A \cap B) = \mathrm{P}(A)\mathrm{P}(B) \iff \mathrm{P}(B | A) = \dfrac{\mathrm{P}(A \cap B)}{\mathrm{P}(A)} = \mathrm{P}(B)$$

# Joint Probability

The joint probability of a pair of random variables is the probability
distribution of all possible combinations of both random variables
considered together, written $\mathrm{P}(A \text{ and } B)$,
$\mathrm{P}(A \cap B)$ or $\mathrm{P}(A, B)$. The _marginal probability
distribution_ of these random variables is the unconditional probability
of one, regardless of the outcome of the other.

The joint probability $p_{X,Y}$ of two random variables $X$ and $Y$ is
given by the unshaded area of the table. The marginal probability
distribution of $X$ and $Y$ are written in the final row and column
respectively.

:::{.wrap-table .joint-probability}

<style>
.joint-probability tbody tr:nth-child(even) {
  background-color: var(--bg);
}

.joint-probability tbody tr:nth-child(odd) {
  background-color: var(--bg);
}

.joint-probability tbody td:nth-child(1) {
  background-color: var(--bg-secondary);
  font-weight: bold;
}
.joint-probability tbody td:nth-last-child(1) {
  background-color: var(--bg-secondary);
}
.joint-probability tbody tr:nth-last-child(1) {
  background-color: var(--bg-secondary);
}

.joint-probability tbody td:nth-last-child(1) {
  background-color: var(--bg-secondary);
}

/* .joint-probability thead tr:nth-child(1) th:nth-child(1) { */
/*   background-color: var(--bg-main) !important; */
/* } */

th,
td {
  border-right: 1px solid var(--bg-emphasis);
  border-bottom: 1px solid var(--bg-emphasis);
}

th:last-child,
td:last-child {
  border-right: none;
}

</style>

| $p_{X,Y}$ |     $X=0$     |     $X=1$     |     $p_Y$     |
| :-------: | :-----------: | :-----------: | :-----------: |
|   $Y=0$   | $\frac{1}{4}$ | $\frac{1}{4}$ | $\frac{1}{2}$ |
|   $Y=1$   | $\frac{1}{4}$ | $\frac{1}{4}$ | $\frac{1}{2}$ |
|   $p_X$   | $\frac{1}{2}$ | $\frac{1}{2}$ |      $1$      |

:::

Events are not assumed to be independent when calculating the joint
probability distribution. Consequently, the intersection of two events
is given by:

$$p_{X,Y}(x,y) = \mathrm{P}(X = x | Y = y)\mathrm{P}(Y=y) = \mathrm{P}(Y = y | X = x)\mathrm{P}(X=x)$$


## Chain Rule of Probability

In the general case, the joint probability of $n$ events is given by the
_chain rule_ of probability:

$$p_{X_1,\ldots,X_n}(x_1,\ldots,x_n) =$$

$$\mathrm{P}(X_n = x_n | X_1 = x_1, \ldots, X_{n-1} = x_{n-1}) \times$$

$$\mathrm{P}(X_{n-1} = x_{n-1} | X_1 = x_1, \ldots, X_{n-1} = x_{n-2}) \times$$

$$\ldots$$ $$\mathrm{P}(X_{2} = x_{2} | X_1 = x_1) \times \mathrm{P}(X_{1} = x_{1})$$

# Bayes' Theorem

Bayes' Theorem facilitates reasoning and inference with probabilities by
updating uncertain _prior_ probability with evidence to calculate the _posterior_
probability.

$$\text{Posterior} = \dfrac{\text{Likelihood} \times \text{Prior}}{\text{Evidence}}$$
$$\mathrm{P}(A | B) = \dfrac{\mathrm{P}(B | A)\mathrm{P}(A)}{\mathrm{P}(B)}$$

Bayes' Theorem can be derived from the definition of conditional
probability.

$$\mathrm{P}(B | A) = \dfrac{\mathrm{P}(A \cap B)}{\mathrm{P}(A)} \iff \mathrm{P}(B | A)\mathrm{P}(A) = \mathrm{P}(A \cap B)$$

$$\mathrm{P}(A | B) = \dfrac{\mathrm{P}(B \cap A)}{\mathrm{P}(B)} \iff \mathrm{P}(A | B) = \dfrac{\mathrm{P}(B | A)\mathrm{P}(A)}{\mathrm{P}(B)} $$
