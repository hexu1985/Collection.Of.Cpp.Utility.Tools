#ifndef __chrono_timer_h
#define __chrono_timer_h

#include <chrono>

class timer {
public:
    using clock_type = std::chrono::steady_clock;
    using time_point = clock_type::time_point;
    using duration = clock_type::duration;

private:
    // starting and ending time measured in clock ticks
    time_point startTime, endTime;

public:
    timer();
    // initialize the timer.
    // Postconditions: set the starting and ending event times to 0.
    // this creates an event whose time is 0.0

    void start();
    // start timing an event.
    // Postcondition:	record the current time as the starting time
    void stop();
    // stop timing an event.
    // Postconditon:	record the current time as the ending time
    double time() const;
    // return the time the event took in seconds by computing
    // the difference between the ending and starting times.
};

// ***********************************************************
//      timer class implementation
// ***********************************************************

// constructor. set starting and ending times to 0
timer::timer():startTime(), endTime()
{}

// determine clock ticks at start
void timer::start()
{
    startTime = clock_type::now();
}

// determine clock ticks at end
void timer::stop()
{
    endTime = clock_type::now();
}

// return the elapsed time in seconds
double timer::time() const
{
    return std::chrono::duration<double>(endTime-startTime).count();
}

#endif
