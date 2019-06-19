#ifndef MINI_UTILS_BASIC_STOPWATCH_H
#define MINI_UTILS_BASIC_STOPWATCH_H

#include <iostream>

/*  class Stopwatch -- simple stopwatch timer for optimization

    Stopwatch prints out elapsed time information for measuring	the run time
    of long-running tasks. Stopwatch can also return the elapsed time without
    printing anything.

    Stopwatch is meant to be used with the RAII idiom. In its
    simplest use, the block

        {
            Stopwatch sw("TheThing()");
            TheThing();
        }

    produces two lines of output like

        TheThing(): start
        // anything printed by TheThing()
        TheThing(): stop 63mS

    Stopwatch is meant to replace something more complex like

    {
        timeval t1, t2;
        long dt;
        gettimeofday(&t1, NULL);
        TheThing();
        gettimeofday(&t2, NULL);
        dt = (t2.tv_sec * 1000000 + t2.tv_usec) - (t1.tv_sec * 1000000 + t1.tv_usec);
        std::cout << "TheThing() took " << dt/1000 << " mS" << std::endl;
    }

    What it prints
        ctor():							"Stopwatch: start"
        ctor(false):					nothing printed
        show():							"Stopwatch: show at xxxx mS"
                                        sets lap time. Get with getLap()
        stop():							"Stopwatch: stop xxxx mS"
                                        sets lap time. Get with getLap()
        stop() when not running:		"Stopwatch: not started"
        start():						"Stopwatch: start"
                                        clears lap time
        start() when running:			"Stopwatch: start xxxx mS"
                                        sets lap time. Get with getLap()
    What it prints when activity name is specified
        ctor("activity"):				"activity: start"
        show("theEvent"):				"activity: theEvent at xxxx mS"
        stop("theEvent"):				"activity: theEvent xxxx mS"
                                        sets lap time. Get with getLap()
        stop("theEvent") not running:	"activity: not started"
        start("theEvent") running:		"activity: theEvent xxxx mS"
                                        sets lap time. Get with getLap()
        start("theEvent") not running: "activity: theEvent"
                                        clears lap time

    printing can be suppressed (get time using getLap()
        ctor("") or ctor(nullptr)		prints nothing
        show("") or show(nullptr)		prints nothing, sets lap time
        start("") or start(nullptr):	prints nothing, sets lap time if running
        stop("") or stop(nullptr):		sets lap time. Get with getLap()
*/

namespace MiniUtils {

template <typename T> 
class BasicStopwatch : public T {
public:
    typedef T BaseTimer;

    // create, optionally start timing an activity
    explicit BasicStopwatch(bool start);
    explicit BasicStopwatch(char const* activity = "Stopwatch",
                             bool start=true);

    BasicStopwatch(std::ostream& log,
                    char const* activity="Stopwatch", 
                    bool start=true); 

    // stop and destroy a stopwatch
    ~BasicStopwatch();

    // get last lap time (time of last stop)
    unsigned long getLap() const;

    // predicate: return true if the stopwatch is running
    bool isStarted() const;

    // show accumulated time, keep running, set/return lap
    unsigned long show(char const* event="show");

    // (re)start a stopwatch, set/return lap time
    unsigned long start(char const* event_namee="start");

    // stop a running stopwatch, set/return lap time
    unsigned long stop(char const* event_name="stop");

private:    //  members
    char const*     activity_; // "activity" string
    unsigned long   lap_;		// lap time (time of last stop or 0)
    std::ostream&   log_;		// stream on which to log events
};

//  performs a start() if start_now == true
template <typename T> 
inline BasicStopwatch<T>::BasicStopwatch(bool start_now)
  : activity_("Stopwatch")
  , lap_(0)
  , log_(std::cout) 
{
    if (start_now)
        start();
}

//	performs a start if start_now == true, suppress print by ctor("")
template <typename T> 
inline BasicStopwatch<T>::BasicStopwatch(char const* activity, bool start_now)
  : activity_(activity && activity[0] ? activity : nullptr)
  , lap_(0)
  , log_(std::cout) 
{
    if (start_now) {
        if (activity_)
            start();
        else
            start(nullptr);
    }
}

//	set log output, optional printout, optional start
template <typename T> 
inline BasicStopwatch<T>::BasicStopwatch(std::ostream& log, char const* activity, bool start_now)
  : activity_(activity && activity[0] ? activity : nullptr)
  , lap_(0)
  , log_(log) 
{
    if (start_now) {
        if (activity_)
            start();
        else
            start(nullptr);
    }
}

//	stop/destroy stopwatch, print message if activity was set in ctor
template <typename T> 
inline BasicStopwatch<T>::~BasicStopwatch() {
    if (isStarted()) {
        if (activity_)
            stop();
        else
            stop(nullptr);
    }
}

//   predicate: return true if the stopwatch is running
template <typename T> 
inline bool BasicStopwatch<T>::isStarted() const
{
    return BaseTimer::isStarted();
}

//	get the last lap time (time of last stop)
template <typename T> 
inline unsigned long BasicStopwatch<T>::getLap() const
{
    return lap_;
}

//   show accumulated time, keep running, get/return lap time
template <typename T> 
inline unsigned long BasicStopwatch<T>::show(char const* event_name) 
{
    if (isStarted()) {
        lap_ = BaseTimer::getMs();
        if (event_name && event_name[0]) {
            if (activity_)
                log_ << activity_ << ": ";
            log_ << event_name << " at " << lap_ << "mS" << std::endl << std::flush;
        }
    } else {
        if (activity_)
            log_ << activity_ << ": not started" << std::endl << std::flush;
    }
    return lap_;
}

//   (re)start a stopwatch, set/return lap time
template <typename T> 
inline unsigned long BasicStopwatch<T>::start(char const* event_name) 
{
    if (isStarted()) {
        stop(event_name);
    } else {
        if (event_name && event_name[0]) {
            if (activity_)
                log_ << activity_ << ": ";
            log_ << event_name << std::endl << std::flush;
        }
    }
    BaseTimer::start();
    return lap_;
}

//   stop a running stopwatch and print the accumulated time
template <typename T> 
inline unsigned long BasicStopwatch<T>::stop(char const* event_name) 
{
    if (isStarted()) {
        lap_ = BaseTimer::getMs();
        if (event_name && event_name[0]) {
            if (activity_)
                log_ << activity_ << ": ";
            log_ << event_name << " " << lap_ << "mS" << std::endl << std::flush;
        }
    }
    BaseTimer::clear();
    return lap_;
}

}   // namespace MiniUtils

#endif
