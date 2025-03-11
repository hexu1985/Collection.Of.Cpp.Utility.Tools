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
    TimerManager() = default;
    ~TimerManager() = default;

    void start();
    void stop();
    bool isRunning();

    void insert(TimerPtr timer);

private:
    void insert_timer(TimerPtr timer);
    void check_timer_once();
    void check_timer_loop();

private:
    std::list<TimerPtr> timer_list_;
    std::mutex timer_list_mutex_;
    std::condition_variable timer_list_cond_;
    TimePoint current_expire_time_point_;
    std::thread timer_check_thread_;
    std::atomic<bool> is_stopped_{false};
};

void TimerManager::start() {
    if (isRunning()) {
        return;
    }

    is_stopped_ = false;
    timer_check_thread_ = std::thread{&TimerManager::check_timer_loop, this};
}

void TimerManager::stop() {
    if (!isRunning()) {
        return;
    }

    is_stopped_ = true;
    timer_list_cond_.notify_one();
    timer_check_thread_.join();
}

bool TimerManager::isRunning() {
    return timer_check_thread_.joinable();
}

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
            insert_timer(timer);
        }
    } else {
        expired = true;
    }

    if (expired) {
        if (timer->active) {
            timer->function();
            if (timer->type == Timer::repeat) {     // for period timer, update expire_time_point
                timer->expire_time_point += timer->interval;
                insert_timer(timer);
            } else {    // for once timer, set timer done
                timer->active = false;
            }
        }
    }
}

void TimerManager::check_timer_loop() {
    while (!is_stopped_) {
        check_timer_once();
    }
}

void TimerManager::insert_timer(TimerPtr timer) {
    auto iter = timer_list_.begin();
    auto end = timer_list_.end();

    while (iter != end) {   // insert timer to timer_list_ by expire_time_point ascending order
        if ((*iter) == timer) { // the timer already in timer_list_
            return;
        }
        if ((*iter)->expire_time_point > timer->expire_time_point) {
            timer_list_.insert(iter, timer);
            break;
        }
    }

    if (iter == end) {  // reached the end of timer_list_
        timer_list_.push_back(timer);
    }

    if (current_expire_time_point_ == TimePoint{} || timer->expire_time_point < current_expire_time_point_) {
        current_expire_time_point_ = timer->expire_time_point;
        timer_list_cond_.notify_one();
    }
}

void TimerManager::insert(TimerPtr timer) {
    if (std::this_thread::get_id() == timer_check_thread_.get_id()) {     // is in timer_check_thread_
        insert_timer(timer);
    } else {
        std::unique_lock<std::mutex> lock{timer_list_mutex_};
        insert_timer(timer);
    }
}

class TimerManagerSingleton: public TimerManager {
public:
    static TimerManagerSingleton& getInstance() {
        static TimerManagerSingleton timer_manager;
        return timer_manager;
    }

private:
    TimerManagerSingleton() {
        start();
    }

    ~TimerManagerSingleton() {
        stop();
    }
};

Timer::Timer(Callback function, Interval interval, Type type) {
    pimpl_ = std::make_shared<Impl>();
    pimpl_->function = function;
    pimpl_->interval = interval;
    pimpl_->type = type;
    pimpl_->expire_time_point = Clock::now() + interval;
    pimpl_->active = true;
    TimerManagerSingleton::getInstance().insert(pimpl_);
}

void Timer::stop() {
    pimpl_->active = false;
}

bool Timer::isActive() {
    return pimpl_->active;
}

