/**
 * @file task_queue.hpp
 * @brief 一个线程安全的支持延时任务的队列类
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2021-01-19
 */
#ifndef MINI_UTILS_DELAY_TASK_QUEUE_INC
#define MINI_UTILS_DELAY_TASK_QUEUE_INC

#include <memory>
#include <queue>
#include <functional>
#include <utility>
#include <mutex>
#include <condition_variable>
#include <chrono>

namespace mini_util {

/**
 * @brief 支持延时任务类的基类
 */
struct TaskBase {
    std::chrono::system_clock::time_point start_time{};

    virtual ~TaskBase() {}

    /**
     * @brief 运行当前任务, 由子类override
     */
    virtual void run() = 0;
};

/**
 * @brief 支持延时任务类
 */
template <typename Fn>
struct Task: public TaskBase {
    /**
     * @brief 从仿函数对象构造一个具体任务类
     *
     * @param fn 仿函数
     */
    Task(Fn &&fn): f_(std::forward<Fn>(fn)) {}

    /**
     * @brief 运行当前任务, 调用具体的仿函数对象
     */
    virtual void run() { f_(); }

    Fn f_;
};

/**
 * @brief 任务类指针
 */
typedef std::shared_ptr<TaskBase> TaskPtr;

/**
 * @brief 创建任务的辅助函数
 *
 * @tparam Fn 任务类中实际要执行的仿函数的类型
 * @param fn 任务类中实际要执行的仿函数
 *
 * @return 任务类指针
 */
template <typename Fn>
std::shared_ptr<Task<Fn>> make_task_aux(Fn &&fn)
{
    return std::make_shared<Task<Fn>>(std::forward<Fn>(fn));
}

/**
 * @brief 创建任务的函数
 *
 * @tparam ...Args 可变长度模板类型
 * @param ...args 可变长度模板参数
 *
 * @return 任务类指针
 */
template <typename ...Args>
std::shared_ptr<TaskBase> make_task(Args &&...args)
{
    return make_task_aux(std::bind(std::forward<Args>(args)...));
}

/**
 * @brief 任务优先级: start_time越靠前(值越小)，优先级越高
 */
struct TaskPriorityPolicy {
    bool operator ()(const std::shared_ptr<TaskBase> &lhs,
        const std::shared_ptr<TaskBase> &rhs) {
        return lhs->start_time > rhs->start_time;
    }
    typedef std::shared_ptr<TaskBase> first_argument_type;
    typedef std::shared_ptr<TaskBase> second_argument_type;
    typedef bool result_type;
};

/**
 * @brief 任务类指针的列表
 */
typedef std::deque<TaskPtr> TaskPtrList;

/**
 * @brief 任务队列
 */
class TaskQueue: 
    public std::priority_queue<std::shared_ptr<TaskBase>, 
            std::vector<std::shared_ptr<TaskBase>>, TaskPriorityPolicy> {
private:
    std::mutex queueMtx_;
    std::condition_variable queueCV_;

public:
    /**
     * @brief 往任务队列里放入一个任务
     *
     * @tparam ...Args 可变长度模板类型
     * @param ...args 可变长度模板参数
     *
     * @note 可以参考std::thread构造函数的用法
     */
    template <typename ...Args>
    void pushTask(Args &&...args)
    {
        this->pushTask(make_task(std::forward<Args>(args)...), 0);
    }

    /**
     * @brief  往任务队列里放入一个任务
     *
     * @param task 任务指针
     * @param delay_ms 延时执行相对时间，单位ms
     */
    void pushTask(std::shared_ptr<TaskBase> task, int delay_ms = 0) 
    {
        std::lock_guard<std::mutex> lck(queueMtx_);
        task->start_time = std::chrono::system_clock::now() + std::chrono::milliseconds(delay_ms);
        this->push(task);
        queueCV_.notify_one();
    }

    /**
     * @brief 唤醒popTask
     */
    void wakeUp()
    {
        std::unique_lock<std::mutex> lck(queueMtx_);
        queueCV_.notify_one();
    }

    /**
     * @brief 最早到时任务时间, 如果队列为空pair的first为false
     *
     * @return 绝对时间
     */
    std::pair<bool, std::chrono::system_clock::time_point> firstTimeUp()
    {
        std::unique_lock<std::mutex> lck(queueMtx_);
        if (this->empty()) {
            return std::make_pair(false, std::chrono::system_clock::time_point{});
        }
        return std::make_pair(true, this->top()->start_time);
    }

    /**
     * @brief 从队列取出一个可以任务
     *
     * @return 任务指针
     */
    std::shared_ptr<TaskBase> popTask() 
    {
        std::unique_lock<std::mutex> lck(queueMtx_);
        while (this->empty() || this->top()->start_time > std::chrono::system_clock::now()) {
            queueCV_.wait(lck);
        }
        auto task = this->top();
        this->pop();
        return task;
    }

    /**
     * @brief 取出当前队列中的所有可以开始的任务
     *
     * @param task_list 任务指针列表
     */
    void popTask(std::deque<std::shared_ptr<TaskBase>> &task_list)
    {
        std::unique_lock<std::mutex> lck(queueMtx_);
        while (this->empty() || this->top()->start_time > std::chrono::system_clock::now()) {
            queueCV_.wait(lck);
        }
        task_list.clear();
        auto now = std::chrono::system_clock::now();
        while (!this->empty()) {
            auto task = this->top();
            if (task->start_time <= now) {
                task_list.push_back(task);
                this->pop();
            } else {
                break;
            }
        }
        return;
    }
};

}   // namespace mini_util

#endif
