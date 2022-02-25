#include "timer.hpp"
#include <list>
#include <mutex>
#include <condition_variable>

namespace mini_util {

class TimerThread {
public:
    TimerThread();
    ~TimerThread(); 

    bool Insert(Timer* timer) {
        std::unique_lock<std::mutex> lck(timer_mutex_);
        return DoInsert(timer);
    }

    bool Remove(Timer* timer) {
        std::unique_lock<std::mutex> lck(timer_mutex_);
        return DoRemove(timer);
    }

    static TimerThread& GetInstance(); 

private:
    void AlarmRoutine();
    bool DoInsert(Timer* timer);
    bool DoRemove(Timer* timer);

private:
    std::list<Timer*> timer_list_;
    std::mutex timer_mutex_;
    std::condition_variable timer_cv_;
    Timer::TimePoint current_timeout_;
};

TimerThread::TimerThread() {
    std::thread(&TimerThread::AlarmRoutine, this).detach();
}

TimerThread::~TimerThread() {
    std::unique_lock<std::mutex> lck(timer_mutex_);
    timer_list_.clear();
}

bool TimerThread::DoInsert(Timer* timer) {
    auto iter = timer_list_.begin();
    auto end = timer_list_.end();
    while (iter != end) {   // 找到合适的插入位置
        if (*iter == timer) {   // timer已经在timer_list_里了
            return false;
        }

        if ((*iter)->timeout_ >= timer->timeout_) {
            break;
        }
        ++iter;
    }

    timer_list_.insert(iter, timer);
    if (current_timeout_ == Timer::TimePoint{} || timer->timeout_ < current_timeout_) {
        current_timeout_ = timer->timeout_;
        timer_cv_.notify_one();
    }
    return true;
}

bool TimerThread::DoRemove(Timer* timer) {
    auto iter = timer_list_.begin();
    auto end = timer_list_.end();
    while (iter != end) {
        if (*iter == timer) {
            break;
        }
        ++iter;
    }

    if (iter == end) {
        return false;
    }
    timer_list_.erase(iter);

    if (timer_list_.empty() || timer->timeout_ == current_timeout_) {
        current_timeout_ = Timer::TimePoint{};
        timer_cv_.notify_one();
    } 

    return true;
}

void TimerThread::AlarmRoutine() {
    std::unique_lock<std::mutex> lck(timer_mutex_);
    while (true) {
        while (timer_list_.empty()) {
            timer_cv_.wait(lck);
        }
        Timer* timer = timer_list_.front();
        auto now = Timer::Clock::now();
        bool expired = false;
        if (timer->timeout_ > now) {
            current_timeout_ = timer->timeout_;
            while (current_timeout_ == timer->timeout_) {
                auto status = timer_cv_.wait_until(lck, timer->timeout_);
                if (status == std::cv_status::timeout) {
                    expired = true;
                    break;
                }
            }
        } else {
            expired = true;
        }

        if (expired) {
            if (DoRemove(timer)) {  // 如果timer已经不在timer_list_里了, 说明外面有人调用Stop
                timer->function_();
                if (timer->interval_ > 0) { // 如果是周期timer, 更新下次超时时间, 重新插入timer_list_
                    timer->timeout_ += std::chrono::milliseconds(timer->interval_);
                    DoInsert(timer);
                }
            }
        }
    }
}

TimerThread& TimerThread::GetInstance() {
    static TimerThread instance;
    return instance;
}

Timer::Timer(): timeout_() {
    TimerThread::GetInstance();
}

Timer::~Timer() {
    TimerThread& timer_thread = TimerThread::GetInstance();
    timer_thread.Remove(this);
}

void Timer::SetTimeout(Callback function, int delay) {
    Stop();
    timeout_ = Clock::now() + std::chrono::milliseconds(delay);
    interval_ = -1;
    function_ = function;
    TimerThread& timer_thread = TimerThread::GetInstance();
    timer_thread.Insert(this);
}

void Timer::SetInterval(Callback function, int interval) {
    Stop();
    timeout_ = Clock::now() + std::chrono::milliseconds(interval);
    interval_ = interval;
    function_ = function;
    TimerThread& timer_thread = TimerThread::GetInstance();
    timer_thread.Insert(this);
}

void Timer::Stop() {
    TimerThread& timer_thread = TimerThread::GetInstance();
    timer_thread.Remove(this);
}

}   // namespace mini_util

