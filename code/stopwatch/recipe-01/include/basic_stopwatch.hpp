/**
 * @file basic_stopwatch.hpp
 * @brief 一个带打印功能的计时器的模板类
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-06-20
 *
 * @note 需要自己实现一个TimerBase类, 具体可参考Stopwathc.hpp
 *
 * @see C++性能优化指南, 3.4节, TimerBaseChrono \n
 * https://github.com/hexu1985/Optimized.Cpp
 */
#ifndef BASIC_STOPWATCH_INC
#define BASIC_STOPWATCH_INC

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
        Ctor():							"Stopwatch: start"
        Ctor(false):					nothing printed
        Show():							"Stopwatch: show at xxxx mS"
                                        sets lap time. Get with GetLap()
        Stop():							"Stopwatch: stop xxxx mS"
                                        sets lap time. Get with GetLap()
        Stop() when not running:		"Stopwatch: not started"
        Start():						"Stopwatch: start"
                                        clears lap time
        Start() when running:			"Stopwatch: start xxxx mS"
                                        sets lap time. Get with GetLap()
    What it prints when activity name is specified
        Ctor("activity"):				"activity: start"
        Show("theEvent"):				"activity: theEvent at xxxx mS"
        Stop("theEvent"):				"activity: theEvent xxxx mS"
                                        sets lap time. Get with GetLap()
        Stop("theEvent") not running:	"activity: not started"
        Start("theEvent") running:		"activity: theEvent xxxx mS"
                                        sets lap time. Get with GetLap()
        Start("theEvent") not running: "activity: theEvent"
                                        clears lap time

    printing can be suppressed (get time using GetLap()
        Ctor("") or Ctor(nullptr)		prints nothing
        Show("") or Show(nullptr)		prints nothing, sets lap time
        Start("") or Start(nullptr):	prints nothing, sets lap time if running
        Stop("") or Stop(nullptr):		sets lap time. Get with GetLap()
*/

/**
 * @brief 一个带打印功能的计时器的模板类
 *
 * @tparam T 实际完成计时功能的基类
 */
template <typename T> 
class BasicStopwatch : public T {
public:
    typedef T BaseTimer;

    /**
     * @brief 创建一个计时器
     *
     * @param start 是否构造时就启动计时
     */
    explicit BasicStopwatch(bool start);

    /**
     * @brief 创建一个计时器
     *
     * @param activity 自定义的activity字符串
     * @param start 是否构造时就启动计时
     */
    explicit BasicStopwatch(char const* activity = "Stopwatch",
                             bool start=true);

    /**
     * @brief 创建一个计时器
     *
     * @param log 使用自定义的输出流
     * @param activity 自定义的activity字符串
     * @param start 是否构造时就启动计时
     */
    BasicStopwatch(std::ostream& log,
                    char const* activity="Stopwatch", 
                    bool start=true); 

    /**
     * @brief 停止并析构计时器
     */
    ~BasicStopwatch();

    /**
     * @brief 获取最后一次lap时间
     *
     * @return 毫秒时间
     */
    unsigned long GetLap() const;

    /**
     * @brief 判断计时器是否已经运行
     *
     * @return 如果已经运行, 返回true, 否则返回false
     */
    bool IsStarted() const;

    /**
     * @brief 显示当前累积时间(从计时器开始), 并更新lap时间
     *
     * @param event 打印时显示的事件信息
     *
     * @return 返回更新后的lap时间
     */
    unsigned long Show(char const* event="show");

    /**
     * @brief 运行计时器, 如果计时器已运行, 先stop
     *
     * @param event_name 打印时显示的事件信息
     *
     * @return 返回stop的lap时间或者0
     */
    unsigned long Start(char const* event_name="start");

    /**
     * @brief 停止计时器
     *
     * @param event_name 打印时显示的事件信息
     *
     * @return 返回stop时的lap时间
     */
    unsigned long Stop(char const* event_name="stop");

private:    //  members
    char const*     activity_; // "activity" string
    unsigned long   lap_;		// lap time (time of last stop or 0)
    std::ostream&   log_;		// stream on which to log events
};

//  performs a Start() if start_now == true
template <typename T> 
inline BasicStopwatch<T>::BasicStopwatch(bool start_now)
  : activity_("Stopwatch")
  , lap_(0)
  , log_(std::cout) 
{
    if (start_now)
        Start();
}

//	performs a start if start_now == true, suppress print by Ctor("")
template <typename T> 
inline BasicStopwatch<T>::BasicStopwatch(char const* activity, bool start_now)
  : activity_(activity && activity[0] ? activity : nullptr)
  , lap_(0)
  , log_(std::cout) 
{
    if (start_now) {
        if (activity_)
            Start();
        else
            Start(nullptr);
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
            Start();
        else
            Start(nullptr);
    }
}

//	stop/destroy stopwatch, print message if activity was set in Ctor
template <typename T> 
inline BasicStopwatch<T>::~BasicStopwatch() {
    if (IsStarted()) {
        if (activity_)
            Stop();
        else
            Stop(nullptr);
    }
}

//   predicate: return true if the stopwatch is running
template <typename T> 
inline bool BasicStopwatch<T>::IsStarted() const
{
    return BaseTimer::IsStarted();
}

//	get the last lap time (time of last stop)
template <typename T> 
inline unsigned long BasicStopwatch<T>::GetLap() const
{
    return lap_;
}

//   show accumulated time, keep running, get/return lap time
template <typename T> 
inline unsigned long BasicStopwatch<T>::Show(char const* event_name) 
{
    if (IsStarted()) {
        lap_ = BaseTimer::GetMs();
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
inline unsigned long BasicStopwatch<T>::Start(char const* event_name) 
{
    if (IsStarted()) {
        Stop(event_name);
    } else {
        if (event_name && event_name[0]) {
            if (activity_)
                log_ << activity_ << ": ";
            log_ << event_name << std::endl << std::flush;
        }
    }
    BaseTimer::Start();
    return lap_;
}

//   stop a running stopwatch and print the accumulated time
template <typename T> 
inline unsigned long BasicStopwatch<T>::Stop(char const* event_name) 
{
    if (IsStarted()) {
        lap_ = BaseTimer::GetMs();
        if (event_name && event_name[0]) {
            if (activity_)
                log_ << activity_ << ": ";
            log_ << event_name << " " << lap_ << "mS" << std::endl << std::flush;
        }
    }
    BaseTimer::Clear();
    return lap_;
}

#endif
