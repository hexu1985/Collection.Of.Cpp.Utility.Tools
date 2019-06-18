/**
 * @file TaskQueue.hpp
 * @brief 一个线程安全的任务队列类
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-06-05
 */
#ifndef MINI_UTILS_TASKQUEUE_INC
#define MINI_UTILS_TASKQUEUE_INC

#include <memory>
#include <deque>
#include <functional>
#include <utility>
#include <mutex>
#include <condition_variable>

namespace MiniUtils {

/**
 * @brief 任务类的基类
 */
struct TaskBase {
    virtual ~TaskBase() {}

    /**
     * @brief 运行当前任务, 由子类override
     */
    virtual void run() = 0;
};

/**
 * @brief 任务类
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
 * @brief 任务类指针的列表
 */
typedef std::deque<TaskPtr> TaskPtrList;

/**
 * @brief 任务队列
 */
class TaskQueue: public std::deque<std::shared_ptr<TaskBase>> {
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
        this->pushTask(make_task(std::forward<Args>(args)...));
    }

    /**
     * @brief 往任务队列里放入一个任务
     *
     * @param task 任务指针
     */
    void pushTask(std::shared_ptr<TaskBase> task) 
    {
        std::lock_guard<std::mutex> lck(queueMtx_);
        this->push_back(task);
        queueCV_.notify_one();
    }

    /**
     * @brief 从队列取出一个任务
     *
     * @return 任务指针
     */
	std::shared_ptr<TaskBase> popTask() 
    {
        std::unique_lock<std::mutex> lck(queueMtx_);
        while (this->empty()) {
            queueCV_.wait(lck);
        }
        auto task = this->front();
        this->pop_front();
        return task;
	}

    /**
     * @brief 取出当前队列中的所有任务
     *
     * @param task_list 任务指针列表
     */
    void popTask(std::deque<std::shared_ptr<TaskBase>> &task_list)
    {
        std::unique_lock<std::mutex> lck(queueMtx_);
        while (this->empty()) {
            queueCV_.wait(lck);
        }
        this->swap(task_list);
    }
};

}   // namespace MiniUtils

#endif
