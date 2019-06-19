#ifndef MINI_UTILS_TIMER_BASE_CHRONO_H
#define MINI_UTILS_TIMER_BASE_CHRONO_H

#include <chrono>

namespace MiniUtils {

template <typename ClockType>
class TimerBaseChrono {
public:
    using TimePointType = typename ClockType::time_point;

	//	clears the timer
	TimerBaseChrono() : start_(TimePointType::min()) { }

	//  clears the timer
	void clear() { 
		start_ = TimePointType::min(); 
	}

	//	returns true if the timer is running
	bool isStarted() const {
		return (start_ != TimePointType::min());
	}

	//	start the timer
	void start()            { start_ = ClockType::now(); }

	//	get the number of milliseconds since the timer was started
	unsigned long getMs() {
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

using TimerBaseSystem = TimerBaseChrono<std::chrono::system_clock>;
using TimerBaseSteady = TimerBaseChrono<std::chrono::steady_clock>;
using TimerBaseHighResolution = TimerBaseChrono<std::chrono::high_resolution_clock>;

}   // namespace MiniUtils

#endif
