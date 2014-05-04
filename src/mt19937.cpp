#include <algorithm>
#include <cassert>
#include "mt19937.hpp"
extern "C" {
#include "pr_mt19937.h"
}

namespace afidd
{
namespace rng
{
class StateOuputIter {
 public:
  StateOuputIter(mt19937_state state) : state_(state), idx_{0} {}
  StateOuputIter(const StateOuputIter& os)=default;
  StateOuputIter& operator=(const StateOuputIter& os)=default;
  StateOuputIter& operator=(unsigned int val) {
    if (idx_<mt19937_N) {
      state_.mt[idx_]=val;
    } else if (idx_==mt19937_N) {
      state_.mti=val;
    } else {
      assert(idx_<mt19937_N+1);
    }
  }
  StateOuputIter& operator*() { return *this; }
  StateOuputIter& operator++() { ++idx_; }

 private:
  mt19937_state& state_;
  size_t idx_;
};
}
}

namespace std {
template<>
struct iterator_traits<afidd::rng::StateOuputIter> {
  typedef double value_type;
};
}

namespace afidd
{
namespace rng
{
class mt19937::impl {
 public:
  impl() {
    mt19937_init_(&state_);
    mt19937_get_sse_state_(&state_, &sse_state_);
  }

  explicit impl(uint64_t seed) {
    // onlyin prand mt19937_dev_init_sequence_(&state_, seed);
    mt19937_init_(&state_);
    mt19937_init_sequence_(&state_, seed);
    mt19937_get_sse_state_(&state_, &sse_state_);
  }

  explicit impl(const impl &o) {
    std::copy(&o.state_.mt[0], &o.state_.mt[mt19937_N], state_.mt);
    state_.mti=o.state_.mti;
    std::copy(&o.sse_state_.mt_aligned[0],
        &o.sse_state_.mt_aligned[3*mt19937_N+5], &sse_state_.mt_aligned[0]);
    std::copy(&o.sse_state_.out[0], &o.sse_state_.out[3*mt19937_N+5],
        &sse_state_.out[0]);
    sse_state_.mti=o.sse_state_.mti;
  }

  impl(std::seed_seq& seq) {
    StateOuputIter os{state_};
    seq.param(os);
    mt19937_get_sse_state_(&state_, &sse_state_);
  }

  impl& operator=(const impl &o) {
    if (this!=&o) {
      std::copy(&o.state_.mt[0], &o.state_.mt[mt19937_N], &state_.mt[0]);
      state_.mti=o.state_.mti;
      std::copy(&o.sse_state_.mt_aligned[0],
          &o.sse_state_.mt_aligned[3*mt19937_N+5], &sse_state_.mt_aligned[0]);
      std::copy(&o.sse_state_.out[0], &o.sse_state_.out[3*mt19937_N+5],
          &sse_state_.out[0]);
      sse_state_.mti=o.sse_state_.mti;
    }
    return *this;
  }

  unsigned int operator()() {
    return mt19937_sse_generate_(&sse_state_);
  }

  void seed(uint64_t seq) {
    mt19937_init_sequence_(&state_, seq);
  }

  void generate(uint32_t* first, uint32_t length) {
    for (uint32_t i=0; i<length; ++i) {
      *first++=mt19937_sse_generate_(&sse_state_);
    }
    //mt19937_generate_array_(&state_, first, length);
  }

  void discard(uint64_t z) {
    mt19937_skipahead_(&state_, z, 0);
  }

  void discard(uint64_t factor, uint32_t power_of_two) {
    mt19937_skipahead_(&state_, factor, power_of_two);
  }

  friend StateOuputIter;
 private:
  mt19937_state state_;
  mt19937_sse_state sse_state_;
};


mt19937::mt19937() : pimpl_{new impl{}} {}
mt19937::mt19937(uint64_t s) : pimpl_{new impl{s}} {}
mt19937::mt19937(const mt19937& engine) : pimpl_{new impl{*engine.pimpl_}} {}
mt19937::mt19937(mt19937&& engine) : pimpl_{std::move(engine.pimpl_)} {}
mt19937::mt19937(std::seed_seq& seq) : pimpl_{new impl{seq}} {}
mt19937::~mt19937() {}

mt19937& mt19937::operator=(const mt19937& o) {
  if (this!=&o) {
    *pimpl_=*o.pimpl_;
  }
  return *this;
}

mt19937& mt19937::operator=(mt19937&& o) {
  if (this!=&o) {
    pimpl_.swap(o.pimpl_);
  }
  return *this;
}

mt19937::result_type mt19937::operator()() {
  return pimpl_->operator()();
}

void mt19937::seed(uint64_t s) {
  pimpl_->seed(s);
}

void mt19937::generate(uint32_t* first, uint32_t* last) {
  pimpl_->generate(first, std::distance(first, last));
}

void mt19937::discard(uint64_t z) {
  pimpl_->discard(z);
}

void mt19937::discard(uint64_t factor, uint32_t power_of_two) {
  pimpl_->discard(factor, power_of_two);
}
}
}
