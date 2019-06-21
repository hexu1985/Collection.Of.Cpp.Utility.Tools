/**
 * @file Stopwatch.hpp
 * @brief 一个基于BasicStopwatch和TimerBaseChrono的计时器类的别名
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-06-21
 */
#ifndef MINI_UTILS_STOPWATCH_H
#define MINI_UTILS_STOPWATCH_H

#include "TimerBaseChrono.hpp"
#include "BasicStopwatch.hpp"

namespace MiniUtils {

template <typename ClockType>
using StopwatchChrono = BasicStopwatch<TimerBaseChrono<ClockType>>;

/**
 * @brief 基于BasicStopwatch和TimerBaseChrono的计时器类的别名
 */
using Stopwatch = StopwatchChrono<std::chrono::system_clock>;

}   // namespace MiniUtils

#endif

