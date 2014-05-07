#ifndef _MC_LFSR_H_
#define _MC_LFSR_H_ 1

#include <memory>
#include <random>
#include <istream>
#include <ostream>

namespace afidd
{
namespace rng
{
class lfsr113 {
 public:
  typedef uint32_t result_type;

  lfsr113();
  ~lfsr113();
  explicit lfsr113(uint64_t seed);
  explicit lfsr113(const lfsr113& engine);
  explicit lfsr113(lfsr113&& engine);
  lfsr113& operator=(const lfsr113& o);
  lfsr113& operator=(lfsr113&& o);
  bool operator==(const lfsr113& b);

  static result_type min() { return 0; }
  static result_type max() { return 0xffffffff; }
  result_type operator()();
  void seed(uint64_t s);

  void generate(uint32_t* first, uint32_t* last);
  void discard(uint64_t z);
  void discard(uint64_t factor, uint32_t power_of_two);
  friend std::ostream& operator<<(std::ostream&, const lfsr113&);
  friend std::istream& operator>>(std::istream&, lfsr113&);
 private:
  class impl
  std::unique_ptr<impl> pimpl_;
};

std::ostream& operator<<(std::ostream&, const lfsr113&);
std::istream& operator>>(std::istream&, lfsr113&);
}
}
#endif
