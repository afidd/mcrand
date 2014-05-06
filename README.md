# mcrand

## Introduction

Random number generation for Monte Carlo in C++.
This library uses recent work by Barash, Yu, and Shchur to ensure
that streams with different seeds are independent, which is crucial for Monte
Carlo techniques. To use this library, you have to download their
library, which is behind an Elsevier paywall. Pick one of these two.

* [RNGSSELIB](http://cpc.cs.qub.ac.uk/summaries/AEIT_v2_0.html) - 
  Barash, L. Yu, and L. N. Shchur. "RNGSSELIB: Program library for random number generation, SSE2 realization." Computer Physics Communications 182.7 (2011): 1518-1527.

* [PRAND](http://cpc.cs.qub.ac.uk/summaries/AESB_v1_0.html) - Requires
  the NVIDIA compiler to compile this.
  Barash, L. Yu, and L. N. Shchur. "PRAND: GPU accelerated parallel random number generation library: Using most reliable algorithms and applying parallelism of modern GPUs and CPUs." Computer Physics Communications (2014).

### Acknowledgements

This library was created by the Analytical Framework for Infectious Disease Dynamics (AFIDD) group at Cornell University in conjunction with the USDA Agricultural Research Service. The AFIDD group is supported by funds from the Department of Homeland Security.

### Availability and Distribution

This library is in the public domain.

## Parallel Random Numbers

### Independent Runs of a Monte Carlo

If we start one MC run with a random seed of 1 and another with a random seed
of 2, then the two streams of pseudorandom numbers are likely highly correlated.
The common technique is therefore to save the state at the end of 
a run to disk and then resume where the previous run left off. Support
for this in the standard library and in Boost comes from the
ostream and istream operators. Once we wish to run at the same time
on more than one thread, process, or machine, then we need parallel random
number generation.


### Standard Methods for Parallel Random Number Generation

There are a few ways to generate parallel random numbers.

* Independent seeding of generators. There are some instances where it is
  possible that generator seeds which are relatively prime may result
  in statistical independence of generators. Because this statement is unsure,
  we typically turn to the methods below.

* Leapfrog method. If there are ten generators, each one gets an index from 0
  to 9 and returns the random number at n*10 + index. For some lagged generators,
  this is quick and easy. The default implementation throws away 9 random numbers.

* Block splitting. Each of the ten generators is created, and then the ith
  generator is advanced by some number, such as 2^(n*i). This requires you to know
  how many random numbers you will use or to keep a running count of those used
  and replenish when needed.

* Generate streams from one generator. Within a single process, it can be
  easy to instantiate one random number generator and make some array of 2^n
  random numbers from which each thread can pull.


## C++ Implementation

### Boost Concepts

The Boost concepts around random number generators are largely adopted
by the standard library. These concepts don't address a lot of what
parallel random generation requires.

* Uniform Random Number Generator

  * The class must have a `result_type` member.

  * The class must be callable with `operator()` and return a sample.

  * The class must have a `min()` operator.

  * The class must have a `max()` operator.

* Pseudo Random Number Generator

  * The class must have a null constructor, `X{}`.

  * The class must have a constructor with user-provided state, `X{...}`.

  * The class must have a `seed(Y)` method for some seed `Y`

### Variation in What Random Number Generators Can Do

*Seeding* The signature for seeding is to use the `result_type`, which
doesn't work well if there are more than 2^32 seeds, which is the case
for many generators.

*Generation of a stream of values* Some random number generators
can be more efficient generating an array of values, possibly because
they are implemented for SSE and GPU. The signature used in 
the standard library is of the form
`template<Iter> void generate(Iter a, Iter b)`. It is therefore up to
the implementation to ensure that boundaries match whatever is convenient
to generate (16 at a time for SSE, for instance).

*Skipahead* The signature for skipahead is `void discard(result_type z)`.
Skips are sometimes limited to powers of two and often more than 2^32,
so this signature might vary from generator to generator.
Also, skipahead is very easy for some generators, such as linear congruential
and more difficult for others, such as Mersenne twister. Sometimes
the skipahead will be written in terms of two or three arguments.

*Serialization* We need to write the state of the random generator to
disk. This is supported with standard iostream interfaces, which is
wonky if you want, say, to write the state to an HDF5 file. You have
to fool the class by treating spaces as separators.

