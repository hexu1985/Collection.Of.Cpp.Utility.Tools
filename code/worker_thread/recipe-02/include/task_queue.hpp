/**
 * @file task_queue.hpp
 * @brief 一个线程安全的任务队列类
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-06-05
 */
#ifndef MINI_UTILS_TASK_QUEUE_INC
#define MINI_UTILS_TASK_QUEUE_INC

#include <memory>
#include <deque>
#include <functional>
#include <utility>
#include <mutex>
#include <condition_variable>

namespace mini_util {

class Task {
public:
    Task() = default;
    ~Task() = default;
    Task(const Task& task) = default;

    /**
     * @brief 运行当前任务, 调用具体的仿函数对象
     */
    void Run() 
    {
        if (task_holder_) {
            task_holder_->Run();
        }
    }

    /**
     * @brief 构造Task对象
     *
     * @param fn 任务运行的函数
     * @param args 任务运行的参数
     */
    template <typename Fn, typename... Args>
    explicit Task(Fn&& fn, Args&&... args) 
    {
        task_holder_ = MakeTaskHolder(std::bind(std::forward<Fn>(fn), std::forward<Args>(args)...));
    }

private:
    /**
     * @brief 任务类的基类
     */
    struct ITaskHolder {
        virtual ~ITaskHolder() {};

        /**
         * @brief 运行当前任务, 由子类override
         */
        virtual void Run() = 0;
    };

    template <typename Fn>
    struct TaskHolderImpl: public ITaskHolder {
        /**
         * @brief 从仿函数对象构造一个具体任务类
         *
         * @param fn 仿函数
         */
        TaskHolderImpl(Fn&& fn): f_(std::forward<Fn>(fn)) {}

        /**
         * @brief 运行当前任务, 调用具体的仿函数对象
         */
        virtual void Run() { f_(); }

        Fn f_;
    };

    /**
     * @brief 创建任务的辅助函数
     *
     * @tparam Fn 任务类中实际要执行的仿函数的类型
     * @param fn 任务类中实际要执行的仿函数
     *
     * @return 任务类指针
     */
    template <typename Fn>
    std::shared_ptr<ITaskHolder> MakeTaskHolder(Fn&& fn) 
    {
        return std::make_shared<TaskHolderImpl<Fn>>(std::forward<Fn>(fn));
    }

private:
    std::shared_ptr<ITaskHolder> task_holder_;
};

/**
 * @brief 任务队列
 */
class TaskQueue: public std::deque<Task> {
private:
    std::mutex queue_mtx_;
    std::condition_variable queue_cv_;

public:
    /**
     * @brief 往任务队列里放入一个任务
     *
     * @param fn 任务运行的函数
     * @param args 任务运行的参数
     *
     * @note 可以参考std::thread构造函数的用法
     */
    template <typename Fn, typename... Args>
    void PushTask(Fn&& fn, Args&&... args)
    {
        this->PushTask(Task(std::forward<Fn>(fn), std::forward<Args>(args)...));
    }

    /**
     * @brief 往任务队列里放入一个任务
     *
     * @param task 任务指针
     */
    void PushTask(Task task) 
    {
        std::lock_guard<std::mutex> lck(queue_mtx_);
        this->push_back(task);
        queue_cv_.notify_one();
    }

    /**
     * @brief 从队列取出一个任务
     *
     * @return 任务指针
     */
    Task PopTask() 
    {
        std::unique_lock<std::mutex> lck(queue_mtx_);
        while (this->empty()) {
            queue_cv_.wait(lck);
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
    void PopTask(std::deque<Task>& task_list)
    {
        std::unique_lock<std::mutex> lck(queue_mtx_);
        while (this->empty()) {
            queue_cv_.wait(lck);
        }
        this->swap(task_list);
    }
};

}   // namespace mini_util

#endif
