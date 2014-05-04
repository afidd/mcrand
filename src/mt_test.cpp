#include <random>
#include <iostream>
#include "mt19937.hpp"

template<typename RNG>
void copy_generate(RNG& rng) {
  RNG rng2{rng};
  bool same=true;
  for (int j=0; j<5; ++j) {
    uint32_t a=rng();
    uint32_t b=rng2();
    if (a!=b) {
      same=false;
      std::cout<< a << "\t" << b << std::endl;
    }
  }
  if (same) {
    std::cout << "Same results" << std::endl;
  }
}

int main(int argc, char* argv[])
{
  using RNG=afidd::rng::mt19937;
  RNG rng;
  copy_generate(rng);

  std::uniform_real_distribution<double> dist(0.0, 1.0);
  double a=1000;
  double b=-1000;
  for (int i=0; i<1000000; ++i) {
    double val=dist(rng);
    if (val<a) a=val;
    if (val>b) b=val;
  }
  std::cout << a << std::endl;
  std::cout << b << std::endl;

  //std::vector<uint32_t> vals{10000, 0};
  //rng.generate(&vals[0], &vals[10000]);

  copy_generate(rng);

	return 0;
}
