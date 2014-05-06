#ifndef _COUNTED_GENERATOR_H_
#define _COUNTED_GENERATOR_H_ 1

namespace afidd
{
namespace rng
{
//! Count the number of times this random number generator was used.
template<typename RNG>
class CountedGenerator {
 public:
  typedef typename RNG::result_type result_type;
  explicit CountedGenerator(RNG& rng) : rng_(rng), cnt_{0} {}
  static result_type min() { return RNG::min(); }
  static result_type max() { return RNG::max(); }
  result_type operator()() {
    ++cnt_;
    return rng_();
  }
  void generate(uint32_t* first, uint32_t* last) {
    cnt_+=std::distance(first, last);
    rng_.generate(first, last);
  }
  uint64_t Count() { return cnt_; }
 private:
  RNG& rng_;
  uint64_t cnt_;
};
}
}


#endif
