#pragma once
#include <autils/log.hpp>

#include "../timer.hpp"

namespace acg::utils {

template <typename Clock> void Timer<Clock>::TickBegin() {
  if (state_ != State::kIdle) UNLIKELY {
      ACG_ERROR("Timer failed to TickBegin. for state is not Idle.");
      return;
    }
  begin_times_.push_back(Clock::now());
  state_ = State::kRunning;
}

template <typename Clock> void Timer<Clock>::TickEnd() {
  if (state_ != State::kRunning) UNLIKELY {
      ACG_ERROR("Timer failed to TickEnd. for state is not Running.");
      state_ = State::kError;
      return;
    }
  end_times_.push_back(Clock::now());
  state_ = State::kIdle;
}

template <typename Clock> Index Timer<Clock>::GetTickCount() const {
  return static_cast<Index>(end_times_.size());
}
template <typename Clock> typename Timer<Clock>::Duration Timer<Clock>::GetLastPeriod() const {
  ACG_DEBUG_CHECK(state_ == State::kIdle, "You can only get last period when the timer is idle.");
  if (GetTickCount() == 0) {
    return Duration::max();
  }

  return end_times_.back() - begin_times_.back();
}

template <typename Clock> void Timer<Clock>::Clear() {
  begin_times_.clear();
  end_times_.clear();
  state_ = State::kIdle;
}

template <typename Clock> bool Timer<Clock>::Good() const { return state_ != State::kError; }

template <typename Clock> class Timer<Clock>::AutoTicker {
  explicit AutoTicker(Timer& timer) : parent_{timer} {
    parent_.TickBegin();
    ACG_DEBUG_CHECK(parent_.Good(), "Cannot auto TickBegin the timer");
  }

  Timer& parent_;

public:
  ~AutoTicker() {
    parent_.TickEnd();
    ACG_DEBUG_CHECK(parent_.Good(), "Cannot auto TickEnd the timer");
  }
};

}  // namespace acg::utils
