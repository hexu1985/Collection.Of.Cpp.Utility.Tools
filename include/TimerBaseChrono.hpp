/**
 * @file TimerBaseChrono.hpp
 * @brief 一个基于std::chrono实现的计时器基类
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-06-20
 *
 * @note 可以作为BasicStopWatch类(BasicStopwatch.hpp)的模板参数, 也可以单独使用
 *
 * @see Optimized C++, Chapter 3, TimerBaseChrono \n
 * https://github.com/hexu1985/Optimized.Cpp
 */
#ifndef MINI_UTILS_TIMER_BASE_CHRONO_INC
#define MINI_UTILS_TIMER_BASE_CHRONO_INC

#include <chrono>

namespace MiniUtils {

/**
 * @brief 基于std::chrono接口实现的计时器
 *
 * @tparam ClockType std:chrono中的clock_type
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
	void clear() 
    { 
		start_ = TimePointType::min(); 
	}

    /**
     * @brief 判断计时器是否已经运行
     *
     * @return 如果已经运行, 返回true, 否则返回false
     */
	bool isStarted() const 
    {
		return (start_ != TimePointType::min());
	}

    /**
     * @brief 运行计时器
     */
	void start()            { start_ = ClockType::now(); }

    /**
     * @brief 获取计时器开始运行到现在经历的时间ms
     *
     * @return 返回ms时间
     */
	unsigned long getMs() 
    {
        using std::chrono::duration_cast;
        using std::chrono::milliseconds;
		if (isStarted()) {
			auto diff = ClockType::now() - start_;
			return (unsigned)(duration_cast<milliseconds>(diff).count());
		}
		return 0;
	}

private:
	TimePointType start_;
}; 

}   // namespace MiniUtils

#endif
