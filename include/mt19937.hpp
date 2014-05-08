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
  //! Each seed yields a statistically-independent set of samples.
  /*! For mt19937, there are 10^19 distinct seeds with 2^440 independent
   *  samples each.
   */
  explicit mt19937(uint64_t seed);
  explicit mt19937(const mt19937& engine);
  explicit mt19937(mt19937&& engine);
  //! The first two 32-bit numbers of the sequence are used to seed.
  /*! There are at least 63 significant bits.
   */
  mt19937(std::seed_seq& seq);
  mt19937& operator=(const mt19937& o);
  mt19937& operator=(mt19937&& o);
  bool operator==(const mt19937& b);

  static result_type min() { return 0; }
  static result_type max() { return 0xffffffff; }
  result_type operator()();
  //! Set the seed.
  void seed(uint64_t s);
  //! Fill an array with uniform values.
  void generate(uint32_t* first, uint32_t* last);
  //! Skip ahead z values in the sequence of samples.
  void discard(uint64_t z);
  //! Skip ahead factor*(2^power_of_two) values in the sequence of samples.
  void discard(uint64_t factor, uint32_t power_of_two);
  friend std::ostream& operator<<(std::ostream&, const mt19937&);
  friend std::istream& operator>>(std::istream&, mt19937&);
 private:
  class impl;
  std::unique_ptr<impl> pimpl_;
};


//! Write the state of the generator to a stream.
/*! This writes spaces between the values which define the stae.
 */
std::ostream& operator<<(std::ostream&, const mt19937&);
//! Read the state of the generator from a stream.
/*! This assumes the values are separated by whitespace,
 *  which is read using the std::ws token.
 */
std::istream& operator>>(std::istream&, mt19937&);
}
}


#endif
