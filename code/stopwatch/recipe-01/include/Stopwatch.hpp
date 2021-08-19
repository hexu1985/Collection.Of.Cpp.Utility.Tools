/**
 * @file Stopwatch.hpp
 * @brief 一个基于BasicStopwatch和TimerBaseChrono的计时器类的别名
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-06-21
 */
#ifndef MINI_UTILS_STOPWATCH_INC
#define MINI_UTILS_STOPWATCH_INC

#include "BasicStopwatch.hpp"

namespace mini_util {

/**
 * @brief 基于std::chrono接口实现的计时器
 */
class TimerBaseChrono {
public:
    using ClockType = std::chrono::system_clock;
    using TimePointType = typename ClockType::time_point;

    /**
     * @brief 创建一个计时器
     */
	TimerBaseChrono() : start_(TimePointType::min()) { }

    /**
     * @brief 重置计时器
     */
	void Clear() 
    { 
		start_ = TimePointType::min(); 
	}

    /**
     * @brief 判断计时器是否已经运行
     *
     * @return 如果已经运行, 返回true, 否则返回false
     */
	bool IsStarted() const 
    {
		return (start_ != TimePointType::min());
	}

    /**
     * @brief 运行计时器
     */
	void Start()            { start_ = ClockType::now(); }

    /**
     * @brief 获取计时器开始运行到现在经历的时间ms
     *
     * @return 返回ms时间
     */
	unsigned long GetMs() 
    {
        using std::chrono::duration_cast;
        using std::chrono::milliseconds;
		if (IsStarted()) {
			auto diff = ClockType::now() - start_;
			return (unsigned)(duration_cast<milliseconds>(diff).count());
		}
		return 0;
	}

private:
	TimePointType start_;
}; 

/**
 * @brief 基于BasicStopwatch和TimerBaseChrono的计时器类的别名
 */
using Stopwatch = BasicStopwatch<TimerBaseChrono>;

}   // namespace mini_util

#endif

