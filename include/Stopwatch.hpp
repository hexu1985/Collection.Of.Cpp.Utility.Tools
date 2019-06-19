#ifndef MINI_UTILS_STOPWATCH_H
#define MINI_UTILS_STOPWATCH_H

#include "TimerBaseChrono.hpp"
#include "BasicStopwatch.hpp"

namespace MiniUtils {

template <typename ClockType>
using StopwatchChrono = BasicStopwatch<TimerBaseChrono<ClockType>>;

using Stopwatch = StopwatchChrono<std::chrono::system_clock>;

}   // namespace MiniUtils

#endif

