mcrand
======

Random number generation for Monte Carlo in C++

This project defines a C++ concept for parallel random
number generation so that a set of threads can each take
an independent random number generator. It works for generators
from std and from Boost.

There is a paper on random number generation 
by Barash and Shchur, http://arxiv.org/abs/1307.5869,
where they describe a modern random number generator capable
of parallel generation and, importantly, independent seeding
of distributions. The code is behind a paywall
at http://cpc.cs.qub.ac.uk/summaries/AEIT_v2_0.html, but if you
get that code, this project will create a C++
class which calls their library and conforms to std
and Boost standards. It makes no
modifications to the library.
