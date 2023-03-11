/****************************************
 * Timers library.
 ****************************************/

#pragma once

#include <acore/common.hpp>
#include <chrono>
#include <vector>

#include "./time.hpp"

namespace acg::utils {

template <typename Clock = std::chrono::steady_clock> class Timer {
public:
  using TimePoint = std::chrono::time_point<Clock>;
  using TimePointList = std::vector<std::chrono::time_point<Clock>>;

  using Duration = typename Clock::duration;
  Timer() = default;

  void TickBegin();

  void TickEnd();

  bool Good() const;

  void Clear();

  Duration GetLastPeriod() const;

  Index GetTickCount() const;

  class AutoTicker;
private:
  TimePointList begin_times_;
  TimePointList end_times_;

  enum class State {
    kRunning,  // has ticked begin. not ticked end.
    kIdle,     // not TickBegin
    kError,    // Last operation failed.
  } state_{State::kIdle};
};

}  // namespace acg::utils

#include "details/timer-inl.hpp"
