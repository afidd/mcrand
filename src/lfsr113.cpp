#include <algorithm>
#include <iterator>
#include <cassert>
extern "C" {
#include "pr_lfsr113.h"
}

namespace afidd
{
namespace rng
{
class lfsr113::impl {
 public:
  impl() {}
 private:
  lfsr113_state state_;
  lfsr113_sse_state sse_state;
};
}
}
