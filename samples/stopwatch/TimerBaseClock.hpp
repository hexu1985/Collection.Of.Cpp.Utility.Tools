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
	void clear()			{ start_ = -1; }

	// returns true if the timer is running
	bool isStarted() const  { return (start_ != -1); }

	// start the timer
	void start()            { start_ = clock(); }

	// get elapsed time in ticks
	tick_t getTicks() {
		if (isStarted()) {
			tick_t now = clock();
			tick_t dt  = (now - start_);
			return dt;
		}
		return 0;
	}

	// get the number of milliseconds since the timer was started
	unsigned getMs() {
		return getMs(getTicks());
	}

	static unsigned getMs(tick_t dt) {
		return (unsigned long)((dt + (500/CLOCKS_PER_SEC)) * (1000 / CLOCKS_PER_SEC));
	}

private:
	tick_t start_;
};

#endif
