#ifndef TIMER_BASE_GET_TIME_OF_DAY_INC
#define TIMER_BASE_GET_TIME_OF_DAY_INC

#include <sys/time.h>

/*
	TimerBaseGettimeofday: timer base class for C++03 and earlier
	start() sets a start time
	getMs() returns the delta time in integer milliseconds
*/
class TimerBaseGettimeofday {
public:
	//	clears the timer
	TimerBaseGettimeofday() { start_.tv_sec = start_.tv_usec = 0; }

	//  clears the timer
	void Clear()			{ start_.tv_sec = start_.tv_usec = 0; }

	//	returns true if the timer is running
	bool IsStarted() const  {
		return (start_.tv_sec != 0) || (start_.tv_usec != 0);
	}

	//	start the timer
	void Start()            { gettimeofday(&start_, NULL); }

	//	get the number of milliseconds since the timer was started
	unsigned GetMs() {
		timeval now;
		if (IsStarted()) {
			gettimeofday(&now, NULL);
			long dt = (now.tv_sec * 1000000 + now.tv_usec) - (start_.tv_sec * 1000000 + start_.tv_usec);
			return (unsigned long)(dt + 500) / 1000; // round to mSec
		}
		return 0;
	}

private:
	timeval start_;
};

#endif
