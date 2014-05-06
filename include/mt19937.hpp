#ifndef _MC_MT19937_H_
#define _MC_MT19937_H_

#include <memory>
#include <random>
#include <istream>
#include <ostream>

namespace afidd
{
namespace rng
{

class mt19937 {
 public:
  typedef uint32_t result_type;

  mt19937();
  ~mt19937();
  //! There are 10^19 distinct seeds each with 10^130 independent samples.
  explicit mt19937(uint64_t seed);
  explicit mt19937(const mt19937& engine);
  explicit mt19937(mt19937&& engine);
  mt19937(std::seed_seq& seq);
  mt19937& operator=(const mt19937& o);
  mt19937& operator=(mt19937&& o);
  bool operator==(const mt19937& b);

  static result_type min() { return 0; }
  static result_type max() { return 0xffffffff; }
  result_type operator()();
  void seed(uint64_t s);

  void generate(uint32_t* first, uint32_t* last);
  void discard(uint64_t z);
  void discard(uint64_t factor, uint32_t power_of_two);
  friend std::ostream& operator<<(std::ostream&, const mt19937&);
  friend std::istream& operator>>(std::istream&, mt19937&);
 private:
  class impl;
  std::unique_ptr<impl> pimpl_;
};


std::ostream& operator<<(std::ostream&, const mt19937&);
std::istream& operator>>(std::istream&, mt19937&);
}
}


#endif
