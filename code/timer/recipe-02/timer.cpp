#include "timer.hpp"
#include <list>
#include <memory>
#include <chrono>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <thread>

#ifdef DEBUG
#include <iostream>
#include <iomanip>
#endif

using Clock = std::chrono::system_clock;
using TimePoint = Clock::time_point; 
using Seconds = std::chrono::seconds;
using AlarmPtr = std::shared_ptr<Timer::Impl>;

#ifdef DEBUG
namespace {

std::ostream& operator<<(std::ostream& out, const TimePoint& tp) {
    using namespace std::chrono;
    auto us = duration_cast<microseconds>(tp.time_since_epoch());
    out << std::fixed << static_cast<double>(us.count())/1000000 << std::defaultfloat;
    return out;
}

template <typename Rep, typename Preiod>
std::ostream& operator<<(std::ostream& out, const std::chrono::duration<Rep, Preiod>& d) {
    using namespace std::chrono;
    auto us = duration_cast<microseconds>(d);
    out << std::fixed << static_cast<double>(us.count())/1000000 << std::defaultfloat;
    return out;
}

}   // namespace
#endif

class Timer::Impl {
public:
    Impl(double interval_, Callback function_): interval(interval_), function(function_) {
    }

    void CalculateTime() {
        time = Clock::now() + std::chrono::microseconds(static_cast<long int>(interval*1000000));
    }

    double interval;
    Timer::Callback function;
    TimePoint time;
    std::atomic<bool> active{true};

#ifdef DEBUG
    std::string message;
#endif
};

class AlarmLooper {
public:
    AlarmLooper() = default; 
    AlarmLooper(const AlarmLooper&) = delete;
    void operator=(const AlarmLooper&) = delete;

    void ThreadSafetyInsert(AlarmPtr alarm) {
        std::unique_lock<std::mutex> lock(alarm_mutex);
        Insert(alarm);
    }

    void Insert(AlarmPtr alarm);
    void Run();
    void Stop();

private:
    std::list<AlarmPtr> alarm_list;
    TimePoint current_alarm;
    std::mutex alarm_mutex;
    std::condition_variable alarm_cond;
    std::atomic<bool> stop{false};
};

/*
 * Insert alarm entry on list, in order.
 */
void AlarmLooper::Insert(AlarmPtr alarm) {
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
     * If we reached the end of the list, insert the new alarm
     * there.  ("next" is NULL, and "last" points to the link
     * field of the last item, or to the list header.)
     */
    if (first == last) {
        alarm_list.push_back(alarm);
    }
#ifdef DEBUG
    std::cout << "[list:";
    for (auto item : alarm_list) {
        std::cout << item->time << "(" << (item->time - Clock::now()) << ")[\""
                << item->message << "\"] ";
    }
    std::cout << "]\n" << std::flush;
#endif
    /*
     * Wake the alarm thread if it is not busy (that is, if
     * current_alarm is 0, signifying that it's waiting for
     * work), or if the new alarm comes before the one on
     * which the alarm thread is waiting.
     */
    if (current_alarm == TimePoint{} || alarm->time < current_alarm) {
        current_alarm = alarm->time;
        alarm_cond.notify_one();
    }
}

/*
 * The alarm thread's start routine.
 */
void AlarmLooper::Run() {
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
        if (stop) return;
        /*
         * If the alarm list is empty, wait until an alarm is
         * added. Setting current_alarm to 0 informs the insert
         * routine that the thread is not busy.
         */
        current_alarm = TimePoint{};
        while (alarm_list.empty()) {
            alarm_cond.wait(lock);
            if (stop) return;
        }
        alarm = alarm_list.front();
        alarm_list.pop_front();
        now = Clock::now();
        expired = false;
        if (alarm->time > now) {
#ifdef DEBUG
            std::cout << "[waiting: " << alarm->time << "(" << (alarm->time - Clock::now()) << ")\""
                << alarm->message << "\"\n" << std::flush; 
#endif
            current_alarm = alarm->time;
            while (current_alarm == alarm->time) {
                status = alarm_cond.wait_until(lock, alarm->time);
                if (status == std::cv_status::timeout) {
                    expired = true;
                    break;
                } 
                if (stop) return;
            }
            if (!expired) {
                Insert(alarm);
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

void AlarmLooper::Stop() {
    stop = true;
    alarm_cond.notify_one();
}

class TimerThread {
public:
    TimerThread(); 
    ~TimerThread(); 

    void AddTimer(std::shared_ptr<Timer::Impl> timer);
    bool CurrentThreadIsAlarmLooperThread() {
        return std::this_thread::get_id() == looper_thread.get_id();
    }

    static TimerThread& GetInstance() {
        static TimerThread timer_thread;
        return timer_thread;
    }

private:
    AlarmLooper alarm_looper;
    std::thread looper_thread;
};

TimerThread::TimerThread() {
    looper_thread = std::thread(&AlarmLooper::Run, &alarm_looper);
}

TimerThread::~TimerThread() {
    alarm_looper.Stop();
    looper_thread.join();
}

void TimerThread::AddTimer(std::shared_ptr<Timer::Impl> timer) {
    if (CurrentThreadIsAlarmLooperThread()) {
        alarm_looper.Insert(timer);
    } else {
        alarm_looper.ThreadSafetyInsert(timer);
    }
}

Timer::Timer(double interval, Callback function) {
    pimpl = std::make_shared<Impl>(interval, function);
}

void Timer::Start() {
    pimpl->CalculateTime();
    TimerThread::GetInstance().AddTimer(pimpl);
}

void Timer::Cancel() {
    pimpl->active = false;
}

#ifdef DEBUG
void Timer::SetMessage(const std::string& message) {
    pimpl->message = message;
}
#endif

