---
title: Information Theory
author: Alexander Neville
date: 2024-02-18
---

Information theory is the study of the storage, communication and
utilisation of _information_. Its applications include the compression,
transmission and analysis of data.

# Information Content

_Self-information_ or _information content_ is a quantity related to the
probability of an event. Self-information represents the uncertainty of
an outcome; the more improbable the outcome, the higher its information
content. For a realisation $x$ of a random variable $X$, according to
distribution $p_X$, the information content is defined to be:

$$\mathrm{I}(x) := -\log_b(p_X(x))$$

The base of the logarithm $b$ is usually one of $2$, $e$ or $10$,
measuring the information content of the event in _bits_ (or _shannon_),
_nats_ (natural unit of information) or _hartleys_ respectively.

# Entropy

The _entropy_ of a random variable is the average information content of
each possible outcome. The entropy of a random variable is the
_expectation_ $\mathbb{E}$ of the information content of each
observation $x_i$ in the image $\mathcal{X} = \{x_1,\ldots,x_n\}$ of
$X$.

$$\mathrm{H}(X) = \mathbb{E}[\mathrm{I}(X)] = -\sum_{x_i \in \mathcal{X}} p(x_i)\log(p(x_i))$$

# Joint Entropy

The _joint entropy_ measures the uncertainty of a set of events.

$$\mathrm{H}(X, Y) = -\sum_{x_i \in \mathcal{X}} \sum_{y_i \in \mathcal{Y}} p(x_i, y_i)\log(p(x_i,y_i))$$

In the general case:

$$\mathrm{H}(X_1,\ldots, X_n) = -\sum_{x_1 \in \mathcal{X}}\ldots \sum_{x_n \in \mathcal{X}} p(x_1,\ldots, x_n)\log(p(x_1, \ldots, x_n))$$

# Conditional Entropy

Much like
[conditional probability](./probability-theory.md#conditional-probability),
the conditional entropy of a random variable $Y$ is the total
information required to describe the uncertainty of its outcome in the
knowledge that event $X$ has occurred.

$$\mathrm{H}(Y| X) = -\sum_{x_i \in \mathcal{X}} \sum_{y_i \in \mathcal{Y}} p(x_i, y_i)\log\left(\dfrac{p(x_i,y_i)}{p(x_i)}\right)$$

Conditional entropy can also be defined
$\mathrm{H}(Y|X) = \mathrm{H}(X,Y) -\mathrm{H}(X)$ using the chain rule
of conditional entropy:

$$\mathrm{H}(Y| X) = -\sum_{x_i \in \mathcal{X}} \sum_{y_i \in \mathcal{Y}} p(x_i, y_i)\log\left(\dfrac{p(x_i,y_i)}{p(x_i)}\right)$$

$$\mathrm{H}(Y| X) = -\sum_{x_i \in \mathcal{X}} \sum_{y_i \in \mathcal{Y}} p(x_i, y_i)(\log(p(x_i,y_i)) - \log(p(x_i)))$$

$$\mathrm{H}(Y| X) = -\sum_{x_i \in \mathcal{X}} \sum_{y_i \in \mathcal{Y}} p(x_i, y_i)\log(p(x_i,y_i)) + \sum_{x_i \in \mathcal{X}} \sum_{y_i \in \mathcal{Y}} p(x_i, y_i)\log(p(x_i))$$

$$\mathrm{H}(Y| X) =  \mathrm{H}(X,Y) - (- \sum_{x_i \in \mathcal{X}} p(x_i)\log(p(x_i)))$$

$$\mathrm{H}(Y| X) =  \mathrm{H}(X,Y) - \mathrm{H}(X)$$

This derivation is hard to follow, but it closely follows the
[chain rule of probability](./probability-theory.md#chain-rule-of-probability),
with products transformed to summation by the logarithms.

# Relative Entropy

The _relative entropy_ or _Kullback–Leibler divergence_ is a measure of
the difference of two probability distributions are. Relative entropy is
not a distance metric as it is not symmetric.

$$D_{\mathrm{KL}}(P \Vert Q) = -\sum_{x_i \in \mathcal{X}} P(x_i)\log\left(\dfrac{Q(x_i)}{P(x_i)}\right)$$

# Mutual Information

The mutual information of two events is the relative entropy or log
difference of the joint entropy and the product of the marginal
probabilities of both events.

$$\mathrm{I}(Y;X) = \sum_{x_i \in \mathcal{X}} \sum_{y_i \in \mathcal{Y}} p(x_i, y_i)\log\left(\dfrac{p(x_i,y_i)}{p(x_i)p(y_i)}\right)$$

Mutual information may also be defined in terms of the joint entropy and
the sum of the marginal entropies:

$$\mathrm{I}(Y;X) = \mathrm{H}(X) + \mathrm{H}(Y) - \mathrm{H}(X,Y)$$
