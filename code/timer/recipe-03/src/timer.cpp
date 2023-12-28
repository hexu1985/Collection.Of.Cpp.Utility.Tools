#include "timer.hpp"
#include <list>
#include <mutex>
#include <condition_variable>

class TimerThread {
public:
    TimerThread();
    ~TimerThread(); 

    bool addTimer(Timer* timer) {
        if (isInAlarmLoopThread()) {
            return insert(timer);
        }

        std::unique_lock<std::mutex> lck(timer_mtx);
        return insert(timer);
    }

    bool deleteTimer(Timer* timer) {
        if (isInAlarmLoopThread()) {
            return remove(timer);
        }

        std::unique_lock<std::mutex> lck(timer_mtx);
        return remove(timer);
    }

    static TimerThread& getInstance(); 

private:
    void alarmLoop();

    //插入Timer，但是不加锁
    bool insert(Timer* timer);

    // 删除Timer，但是不加锁
    bool remove(Timer* timer);

    bool isInAlarmLoopThread()
    {
        return std::this_thread::get_id() == loop_thread_id; 
    }

private:
    std::thread::id loop_thread_id;
    std::atomic<bool> active{true};
    std::mutex timer_mtx;
    std::condition_variable timer_cv;
    std::list<Timer*> timer_list;
    Timer::TimePoint current_timeout;
};

TimerThread::TimerThread() {
    std::thread loop_thread = std::thread(&TimerThread::alarmLoop, this);
    loop_thread_id = loop_thread.get_id();
    loop_thread.detach();
}

TimerThread::~TimerThread() {
    if (isInAlarmLoopThread()) {
        timer_list.clear();
        return;
    }

    std::unique_lock<std::mutex> lck(timer_mtx);
    timer_list.clear();
}

bool TimerThread::insert(Timer* timer) {
    auto iter = timer_list.begin();
    auto end = timer_list.end();
    while (iter != end) {       // 找到合适的插入位置
        if (*iter == timer) {   // timer已经在timer_list里了
            return false;
        }

        if (timer->timeout < (*iter)->timeout) {  // 找到timer_list里第一个大于当前timer超时时间的timer
            break;
        }
        ++iter;
    }

    timer_list.insert(iter, timer);
    if (current_timeout == Timer::TimePoint{} || timer->timeout < current_timeout) {
        current_timeout = timer->timeout;
        timer_cv.notify_one();
    }
    return true;
}

bool TimerThread::remove(Timer* timer) {
    auto iter = timer_list.begin();
    auto end = timer_list.end();
    while (iter != end) {
        if (*iter == timer) {
            break;
        }
        ++iter;
    }

    if (iter == end) {
        return false;
    }
    timer_list.erase(iter);

    if (timer_list.empty() || timer->timeout == current_timeout) {
        current_timeout = Timer::TimePoint{};
        timer_cv.notify_one();
    } 

    return true;
}

void TimerThread::alarmLoop() {
    std::unique_lock<std::mutex> lck(timer_mtx);
    while (true) {
        while (timer_list.empty()) {
            timer_cv.wait(lck);
        }
        Timer* timer = timer_list.front();
        auto now = Timer::Clock::now();
        bool expired = false;
        if (timer->timeout > now) {
            current_timeout = timer->timeout;
            while (current_timeout == timer->timeout) {
                auto status = timer_cv.wait_until(lck, timer->timeout);
                if (status == std::cv_status::timeout) {
                    expired = true;
                    break;
                }
            }
        } else {
            expired = true;
        }

        if (expired) {
            if (remove(timer)) {  // 如果timer已经不在timer_list里了, 说明外面有人调用stop
                timer->function();
                if (timer->interval > 0) { // 如果是周期timer, 更新下次超时时间, 重新插入timer_list
                    timer->timeout += std::chrono::milliseconds(timer->interval);
                    insert(timer);
                }
            }
        }
    }
}

TimerThread& TimerThread::getInstance() {
    static TimerThread instance;
    return instance;
}

Timer::Timer(): timeout() {
    TimerThread::getInstance();
}

Timer::~Timer() {
//    TimerThread& timer_thread = TimerThread::getInstance();
//    timer_thread.deleteTimer(this);
}

void Timer::setTimeout(Callback function, int delay) {
    stop();
    this->timeout = Clock::now() + std::chrono::milliseconds(delay);
    this->interval = -1;
    this->function = function;
    TimerThread& timer_thread = TimerThread::getInstance();
    timer_thread.addTimer(this);
}

void Timer::setInterval(Callback function, int interval) {
    stop();
    this->timeout = Clock::now() + std::chrono::milliseconds(interval);
    this->interval = interval;
    this->function = function;
    TimerThread& timer_thread = TimerThread::getInstance();
    timer_thread.addTimer(this);
}

void Timer::stop() {
    TimerThread& timer_thread = TimerThread::getInstance();
    timer_thread.deleteTimer(this);
}

