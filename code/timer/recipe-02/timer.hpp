#pragma once

#include <iostream>
#include <chrono>
#include <atomic>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <thread>
using Clock = std::chrono::system_clock;
using TimePoint = Clock::time_point; 

class TimerThread;

class Timer {
public:
    typedef std::function<void ()> Callback;
    typedef std::chrono::system_clock Clock;
    typedef Clock::time_point TimePoint;

    Timer();
    ~Timer();

    void setTimeout(Callback function, int delay_ms)    // unit millisecond
    {
        start_timer(std::move(function), static_cast<double>(delay_ms), false);
    }

    void setInterval(Callback function, int interval_ms)  // unit millisecond
    {
        start_timer(std::move(function), static_cast<double>(interval_ms), true);
    }

    void stop();

public:
    struct Impl; 

private:
    void start_timer(Callback function, double interval, bool is_period); // unit second

private:
    std::shared_ptr<Impl> pimpl;
    std::shared_ptr<TimerThread> timer_thread; 
};

class AlarmLooper {
public:
    AlarmLooper() = default; 
    AlarmLooper(const AlarmLooper&) = delete;
    void operator=(const AlarmLooper&) = delete;

    void thread_safety_insert(std::shared_ptr<Timer::Impl> alarm) {
        std::unique_lock<std::mutex> lock(alarm_mutex);
        insert(alarm);
    }

    void insert(std::shared_ptr<Timer::Impl> alarm);
    void run();
    void stop();

private:
    std::list<std::shared_ptr<Timer::Impl>> alarm_list;
    TimePoint current_alarm;
    std::mutex alarm_mutex;
    std::condition_variable alarm_cond;
    std::atomic<bool> stopped{false};
};

class TimerThread {
public:
    TimerThread()
    {
        looper_thread = std::thread(&AlarmLooper::run, &alarm_looper);
    }
    ~TimerThread()
    {
        alarm_looper.stop();
        looper_thread.join();
    }

    void insert_alarm(std::shared_ptr<Timer::Impl> timer);
    bool is_in_looper_thread() {
        return std::this_thread::get_id() == looper_thread.get_id();
    }


private:
    AlarmLooper alarm_looper;
    std::thread looper_thread;
};


