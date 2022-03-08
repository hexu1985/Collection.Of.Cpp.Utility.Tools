#ifndef TIMER_BASE_CLOCK_INC
#define TIMER_BASE_CLOCK_INC

#include <ctime>

/*
	TimerBaseClock: timer base class for C++03 and eariler
	start() sets a start time
	getMs() returns the delta time in integer milliseconds
*/
class TimerBaseClock {
public:
	// tick type
	typedef clock_t tick_t;

	// clears the timer
	TimerBaseClock()		{ start_ = -1; }

	// clears the timer
	void Clear()			{ start_ = -1; }

	// returns true if the timer is running
	bool IsStarted() const  { return (start_ != -1); }

	// start the timer
	void Start()            { start_ = clock(); }

	// get elapsed time in ticks
	tick_t GetTicks() {
		if (IsStarted()) {
			tick_t now = clock();
			tick_t dt  = (now - start_);
			return dt;
		}
		return 0;
	}

	// get the number of milliseconds since the timer was started
	unsigned GetMs() {
		return GetMs(GetTicks());
	}

	static unsigned GetMs(tick_t dt) {
		return (unsigned long)((dt + (500/CLOCKS_PER_SEC)) * (1000 / CLOCKS_PER_SEC));
	}

private:
	tick_t start_;
};

#endif
