#include "timer.hpp"
#include <list>
#include <memory>
#include <chrono>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <thread>

using Clock = std::chrono::system_clock;
using TimePoint = Clock::time_point; 
using Seconds = std::chrono::seconds;
using Microseconds = std::chrono::microseconds;

class Timer::Impl {
public:
    Impl(double interval_, Callback function_): interval(interval_), function(function_) {
    }

    void set_alarm_time() {
        time = Clock::now() + Microseconds(static_cast<long int>(interval*1000000));
    }

    double interval;            // unit second
    Timer::Callback function;   // callback function
    TimePoint time;             // expiration time
    std::atomic<bool> active{true};
};

using Alarm = Timer::Impl;
using AlarmPtr = std::shared_ptr<Alarm>;

class AlarmLooper {
public:
    AlarmLooper() = default; 
    AlarmLooper(const AlarmLooper&) = delete;
    void operator=(const AlarmLooper&) = delete;

    void insert_alarm(AlarmPtr alarm) {
        std::lock_guard<std::mutex> lock(alarm_mutex);
        insert_alarm_non_locking(alarm);
    }

    void insert_alarm_non_locking(AlarmPtr alarm);
    void loop();
    void stop();

private:
    std::list<AlarmPtr> alarm_list;
    TimePoint current_alarm_time;
    std::mutex alarm_mutex;
    std::condition_variable alarm_cond;
    std::atomic<bool> stopped{false};
};

/*
 * Insert alarm entry on list, in order.
 */
void AlarmLooper::insert_alarm_non_locking(AlarmPtr alarm) {
    auto first = alarm_list.begin();
    auto last = alarm_list.end();

    /*
     * LOCKING PROTOCOL:
     * 
     * This routine requires that the caller have locked the
     * alarm_mutex!
     */
    for ( ; first != last; ++first) {
        if ((*first)->time >= alarm->time) {
            alarm_list.insert(first, alarm);
            break;
        }
    }

    /*
     * If we reached the end of the list, insert the new alarm there. 
     */
    if (first == last) {
        alarm_list.push_back(alarm);
    }

    /*
     * Wake the alarm thread if it is not busy (that is, if
     * current_alarm_time is 0, signifying that it's waiting for
     * work), or if the new alarm comes before the one on
     * which the alarm thread is waiting.
     */
    if (current_alarm_time == TimePoint{} || alarm->time < current_alarm_time) {
        current_alarm_time = alarm->time;
        alarm_cond.notify_one();
    }
}

/*
 * The alarm thread's start routine.
 */
void AlarmLooper::loop() {
    AlarmPtr alarm;
    TimePoint now;
    bool expired;
    std::cv_status status;

    /*
     * Loop forever, processing commands. The alarm thread will
     * be disintegrated when the process exits. Lock the mutex
     * at the start -- it will be unlocked during condition
     * waits, so the main thread can insert alarms.
     */
    std::unique_lock<std::mutex> lock(alarm_mutex);
    while (true) {
        if (stopped) return;
        /*
         * If the alarm list is empty, wait until an alarm is
         * added. Setting current_alarm_time to 0 informs the insert
         * routine that the thread is not busy.
         */
        current_alarm_time = TimePoint{};
        while (alarm_list.empty()) {
            alarm_cond.wait(lock);
            if (stopped) return;
        }

        alarm = alarm_list.front();
        alarm_list.pop_front();
        now = Clock::now();
        expired = false;
        if (alarm->time > now) {
            current_alarm_time = alarm->time;
            while (current_alarm_time == alarm->time) {
                status = alarm_cond.wait_until(lock, alarm->time);
                if (status == std::cv_status::timeout) {
                    expired = true;
                    break;
                } 
                if (stopped) return;
            }
            if (!expired) {
                insert_alarm_non_locking(alarm);
            }
        } else {
            expired = true;
        }

        if (expired) {
            if (alarm->active) {
                alarm->function();
            }
        }
    }
}

void AlarmLooper::stop() {
    stopped = true;
    alarm_cond.notify_one();
}

class TimerThread {
public:
    TimerThread(); 
    ~TimerThread(); 

    void add_timer(std::shared_ptr<Timer::Impl> timer);
    bool is_current_thread_looper_thread() {
        return std::this_thread::get_id() == looper_thread.get_id();
    }

    static TimerThread& get_instance() {
        static TimerThread timer_thread;
        return timer_thread;
    }

private:
    AlarmLooper looper;
    std::thread looper_thread;
};

TimerThread::TimerThread() {
    looper_thread = std::thread(&AlarmLooper::loop, &looper);
}

TimerThread::~TimerThread() {
    looper.stop();
    looper_thread.join();
}

void TimerThread::add_timer(std::shared_ptr<Timer::Impl> timer) {
    if (is_current_thread_looper_thread()) {
        looper.insert_alarm_non_locking(timer);
    } else {
        looper.insert_alarm(timer);
    }
}

Timer::Timer(double interval, Callback function) {
    pimpl = std::make_shared<Alarm>(interval, function);
}

void Timer::start() {
    pimpl->set_alarm_time();
    TimerThread::get_instance().add_timer(pimpl);
}

void Timer::cancel() {
    pimpl->active = false;
}

