mcrand
======

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

This library makes C++ classes which conform to the random number generator
concept.

