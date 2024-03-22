#include "timer.hpp"
using Microseconds = std::chrono::microseconds;
using AlarmPtr = std::shared_ptr<Timer::Impl>;

class Timer::Impl {
public:
    Impl() {
    }

    ~Impl() {
    }

    bool is_valid() {
        return !(time == TimePoint{});
    }

    void setup_alarm(double interval_, bool is_period_, Callback function_) {
        interval = interval_;
        is_period = is_period_;
        function = function_;
        time = Clock::now() + Microseconds(static_cast<long int>(interval*1000));
        active = true;
    }

    void update_alarm() {
        time = time + Microseconds(static_cast<long int>(interval*1000));
    }

    double interval{0.0};       // unit second
    bool is_period{false};     // is period
    Timer::Callback function;   // callback function
    TimePoint time;             // expiration time
    std::atomic<bool> active{false};
    bool is_executing{false};
};



/*
 * Insert alarm entry on list, in order.
 */
void AlarmLooper::insert(AlarmPtr alarm) {
    auto first = alarm_list.begin();
    auto last = alarm_list.end();

    /*
     * LOCKING PROTOCOL:
     * 
     * This routine requires that the caller have locked the
     * alarm_mutex!
     */
    for ( ; first != last; ++first) {
        if ((*first) == alarm) {    // already in alarm list
            return;
        }
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
void AlarmLooper::run() {
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
         * added. Setting current_alarm to 0 informs the insert
         * routine that the thread is not busy.
         */
        current_alarm = TimePoint{};
        while (alarm_list.empty()) {
            alarm_cond.wait(lock);
            if (stopped) return;
        }

        alarm = alarm_list.front();
        alarm_list.pop_front();
        now = Clock::now();
        expired = false;
        if (alarm->time > now) {
            current_alarm = alarm->time;
            while (current_alarm == alarm->time) {
                status = alarm_cond.wait_until(lock, alarm->time);
                if (status == std::cv_status::timeout) {
                    expired = true;
                    break;
                } 
                if (stopped) return;
            }
            if (!expired) {
                insert(alarm);
            }
        } else {
            expired = true;
        }

        if (expired) {
            if (alarm->active && !alarm->is_executing) {
                alarm->is_executing = true;
                alarm->function();
                alarm->is_executing = false;
                if (alarm->is_period) {
                    alarm->update_alarm();
                    insert(alarm);
                }
            }
        }
    }
}

void AlarmLooper::stop() {
    stopped = true;
    alarm_cond.notify_one();
}




void TimerThread::insert_alarm(std::shared_ptr<Timer::Impl> pimpl) {
    if (is_in_looper_thread()) {
        alarm_looper.insert(pimpl);
    } else {
        alarm_looper.thread_safety_insert(pimpl);
    }
}

Timer::Timer() {
    pimpl = std::make_shared<Impl>();
}

Timer::~Timer() {
}

void Timer::start_timer(Callback function, double interval, bool is_period) {
    if (pimpl->is_valid()) {
        return;
    }
    pimpl->setup_alarm(interval, is_period, function);
    if (!timer_thread) {
        timer_thread = std::make_shared<TimerThread>(); // 创建 TimerThread 对象
    }
    timer_thread->insert_alarm(pimpl);

}

void Timer::stop() {
    pimpl->active = false;
}

