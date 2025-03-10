#include "timer.hpp"
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <list>

using Clock = std::chrono::system_clock;
using TimePoint = Clock::time_point;

struct Timer::Impl {
    Timer::Callback function;
    Timer::Interval interval;
    Timer::Type type;
    TimePoint expire_time_point;
    std::atomic<bool> active{false};
};

using TimerPtr = std::shared_ptr<Timer::Impl>;

class TimerManager {
public:
    void start();
    void stop();

    bool insert(TimerPtr timer);

private:
    void timer_insert(TimerPtr timer);
    void check_timer_once();
    void timer_loop();

private:
    std::list<TimerPtr> timer_list_;
    std::mutex timer_list_mutex_;
    std::condition_variable timer_list_cond_;
    TimePoint current_expire_time_point_;
    std::thread timer_check_thread_;
    std::atomic<bool> is_stopped_{false};
};

void TimerManager::check_timer_once() {
    std::unique_lock<std::mutex> lock{timer_list_mutex_};
    current_expire_time_point_ = TimePoint{};

    while (timer_list_.empty()) {
        timer_list_cond_.wait(lock);
        if (is_stopped_) return;        // thread cancel check point
    }

    // pop first timer of timer_list_
    auto timer = timer_list_.front();
    timer_list_.pop_front();

    auto now = Clock::now();
    bool expired = false;
    if (timer->expire_time_point > now) {
        current_expire_time_point_ = timer->expire_time_point;
        while (current_expire_time_point_ == timer->expire_time_point) {
            auto status = timer_list_cond_.wait_until(lock, timer->expire_time_point);
            if (status == std::cv_status::timeout) {
                expired = true;
                break;
            }
            if (is_stopped_) return;        // thread cancel check point
        }
        if (!expired) {     // perhaps an new timer insert into the timer_list_
            timer_insert(timer);
        }
    } else {
        expired = true;
    }

    if (expired) {
        if (timer->active) {
            timer->function();
            if (timer->type == Timer::repeat) {     // for period timer, update expire_time_point
                timer->expire_time_point += timer->interval;
                timer_insert(timer);
            }
        }
    }
}

void TimerManager::timer_loop() {
    while (!is_stopped_) {
        check_timer_once();
    }
}

Timer::Timer(Callback function, Interval interval, Type type) {
    pimpl_ = std::make_shared<Impl>();
    pimpl_->function = function;
    pimpl_->interval = interval;
    pimpl_->type = type;
    pimpl_->expire_time_point = Clock::now() + interval;
    pimpl_->active = true;
}

void Timer::stop() {
}

bool Timer::isRunning() {
}

