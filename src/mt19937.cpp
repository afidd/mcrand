#include <algorithm>
#include <iterator>
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
  impl() : sse_synchronized_{true} {
    mt19937_init_(&state_);
    mt19937_get_sse_state_(&state_, &sse_state_);
  }

  explicit impl(uint64_t seed) : sse_synchronized_{true} {
    // onlyin prand mt19937_dev_init_sequence_(&state_, seed);
    mt19937_init_(&state_);
    mt19937_init_sequence_(&state_, seed);
    mt19937_get_sse_state_(&state_, &sse_state_);
  }

  explicit impl(const impl &o) : sse_synchronized_{true} {
    std::copy(&o.state_.mt[0], &o.state_.mt[mt19937_N], state_.mt);
    state_.mti=o.state_.mti;
    std::copy(&o.sse_state_.mt_aligned[0],
        &o.sse_state_.mt_aligned[3*mt19937_N+5], &sse_state_.mt_aligned[0]);
    std::copy(&o.sse_state_.out[0], &o.sse_state_.out[3*mt19937_N+5],
        &sse_state_.out[0]);
    sse_state_.mti=o.sse_state_.mti;
  }

  impl(std::seed_seq& seq) : sse_synchronized_{true} {
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

  bool operator==(const impl& o) {
    bool s=std::equal(&state_.mt[0], &state_.mt[mt19937_N], &o.state_.mt[0]);
    bool mt=(state_.mt-&state_.mt[0]==o.state_.mt-&o.state_.mt[0]);
    bool sse=std::equal(&sse_state_.mt_aligned[0],
        &sse_state_.mt_aligned[3*mt19937_N+5], &o.sse_state_.mt_aligned[0]);
    bool out=std::equal(&sse_state_.out[0], &sse_state_.out[3*mt19937_N+5],
        &o.sse_state_.out[0]);
    bool mtb=(sse_state_.mt-&sse_state_.mt_aligned[0])==
        (o.sse_state_.mt-&o.sse_state_.mt_aligned[0]);
    bool mti=(sse_state_.mti==o.sse_state_.mti);
    return s && mt && sse && out && mtb && mti;
  }

  unsigned int operator()() {
    sse_synchronized_=false;
    return mt19937_sse_generate_(&sse_state_);
  }

  void seed(uint64_t seq) {
    assert(sse_synchronized_);
    mt19937_init_sequence_(&state_, seq);
  }

  void generate(uint32_t* first, uint32_t length) {
    sse_synchronized_=false;
    for (uint32_t i=0; i<length; ++i) {
      *first++=mt19937_sse_generate_(&sse_state_);
    }
    //mt19937_generate_array_(&state_, first, length);
  }

  void discard(uint64_t z) {
    assert(sse_synchronized_);
    mt19937_skipahead_(&state_, z, 0);
  }

  void discard(uint64_t factor, uint32_t power_of_two) {
    assert(sse_synchronized_);
    mt19937_skipahead_(&state_, factor, power_of_two);
  }

  friend StateOuputIter;

  inline friend std::ostream& operator<<(std::ostream& os, const impl& mt) {
    for (int i=0; i<mt19937_N; ++i) {
      os << mt.state_.mt[i] << ' ';
    }
    os << mt.state_.mti << ' ';
    for (int j=0; j<3*mt19937_N+5; ++j) {
      os << mt.sse_state_.mt_aligned[j] << ' ';
    }
    for (int k=0; k<3*mt19937_N+5; ++k) {
      os << mt.sse_state_.out[k] << ' ';
    }
    os << mt.sse_state_.mti << ' ';
    os << mt.sse_state_.mt-&mt.sse_state_.mt_aligned[0] << ' ';
    return os << mt.sse_synchronized_;
  }

  inline friend std::istream& operator>>(std::istream& is, impl& mt) {
    for (int i=0; i<mt19937_N; ++i) {
      is >> mt.state_.mt[i] >> std::ws;
    }
    is >> mt.state_.mti >> std::ws;
    for (int j=0; j<3*mt19937_N+5; ++j) {
      is >> mt.sse_state_.mt_aligned[j] >> std::ws;
    }
    for (int k=0; k<3*mt19937_N+5; ++k) {
      is >> mt.sse_state_.out[k] >> std::ws;
    }
    is >> mt.sse_state_.mti >> std::ws;
    uint64_t cnt{0};
    is >> cnt >> std::ws;
    mt.sse_state_.mt=&mt.sse_state_.mt_aligned[0];
    std::advance(mt.sse_state_.mt, cnt);
    return is >> mt.sse_synchronized_;
  }
 private:
  mt19937_state state_;
  mt19937_sse_state sse_state_;
  // Once sse_state_ is used to generate a random number the two
  // states are unsynchronized. Skipahead only works for state_,
  // so this checks whether the two are still in sync.
  bool sse_synchronized_;
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

bool mt19937::operator==(const mt19937& o) {
  return *pimpl_==*o.pimpl_;
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


std::ostream& operator<<(std::ostream& os, const mt19937& mt) {
  return os << *mt.pimpl_;
}

std::istream& operator>>(std::istream& is, mt19937& mt) {
  return is >> *mt.pimpl_;
}
}
}
