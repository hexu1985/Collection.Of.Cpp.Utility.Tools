# ifndef STOPWATCH03_H
#	define STOPWATCH03_H

#	include <time.h>
	/*
		TimerBaseClock: timer base class for C++03 and eariler
		Start() sets a start time
		GetMs() returns the delta time in integer milliseconds
	*/
	class TimerBaseClock {
	public:
		// tick type
		typedef clock_t tick_t;

		// clears the timer
		TimerBaseClock()		{ m_start = -1; }

		// clears the timer
		void Clear()			{ m_start = -1; }

		// returns true if the timer is running
		bool IsStarted() const  { return (m_start != -1); }

		// start the timer
		void Start()            { m_start = clock(); }

		// get elapsed time in ticks
		tick_t GetTicks() {
			if (IsStarted()) {
				tick_t now = clock();
				tick_t dt  = (now - m_start);
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
		tick_t m_start;
	};

#	include "gettimeofday.h"
	/*
		TimerBaseGetTimeOfDay: timer base class for C++03 and earlier
		Start() sets a start time
		GetMs() returns the delta time in integer milliseconds
	*/
	class TimerBaseGetTimeOfDay {
	public:
		//	clears the timer
		TimerBaseGetTimeOfDay() { m_start.tv_sec = m_start.tv_usec = 0; }

		//  clears the timer
		void Clear()			{ m_start.tv_sec = m_start.tv_usec = 0; }

		//	returns true if the timer is running
		bool IsStarted() const  {
			return (m_start.tv_sec != 0) || (m_start.tv_usec != 0);
		}

		//	start the timer
		void Start()            { gettimeofday(&m_start, NULL); }

		//	get the number of milliseconds since the timer was started
		unsigned GetMs() {
			timeval now;
			if (IsStarted()) {
				gettimeofday(&now, NULL);
				long dt = (now.tv_sec * 1000000 + now.tv_usec) - (m_start.tv_sec * 1000000 + m_start.tv_usec);
				return (unsigned long)(dt + 500) / 1000; // round to mSec
			}
			return 0;
		}
	private:
		timeval m_start;
	};

#	include "stopwatch.h"
	typedef basic_stopwatch<TimerBaseClock> Stopwatch;
# endif
