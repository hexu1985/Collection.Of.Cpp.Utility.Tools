/**
 * @file Stopwatch.hpp
 * @brief 一个基于BasicStopwatch和TimerBaseChrono的计时器类的别名
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-06-21
 */
#ifndef MINI_UTILS_STOPWATCH_INC
#define MINI_UTILS_STOPWATCH_INC

#include "TimerBaseChrono.hpp"
#include "BasicStopwatch.hpp"

namespace mini_utils {

/**
 * @brief 基于BasicStopwatch和TimerBaseChrono的计时器类的别名
 */
using Stopwatch = BasicStopwatch<TimerBaseChrono>;

}   // namespace mini_utils

#endif

