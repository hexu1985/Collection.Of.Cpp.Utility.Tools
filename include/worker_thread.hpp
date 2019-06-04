#ifndef GUL_MINI_UTILS_THREAD_INC
#define GUL_MINI_UTILS_THREAD_INC

#include <thread>
#include <string>

#include "task_queue.hpp"

namespace MiniUtils {

/**
 * @brief 基于TaskQueue的工作线程类
 */
class WorkerThread {
public:
    /**
     * @brief 构造一个工作线程类对象
     *
     * @param name 线程名
     */
    WorkerThread(const std::string &name);

    /**
     * @brief 析构工作线程类对象
     */
    ~WorkerThread();

    /**
     * @brief 创建一个TaskQueue, 以及一个工作线程处理TaskQueue的任务
     */
    void start();

    /**
     * @brief 停止工作线程, 会把TaskQueue中已有的任务都处理完, 才返回
     */
    void stop();

    /**
     * @brief 工作线程是否运行
     *
     * @return 如果工作线程正在运行, 返回true; 否则返回false
     */
    bool is_run();

    /**
     * @brief 获取TaskQueue
     *
     * @return TaskQueue的指针
     */
    std::shared_ptr<TaskQueue> get_task_queue();

    /**
     * @brief 获取工作线程名
     *
     * @return 线程名
     */
    const std::string &get_name() const;

private:
    WorkerThread(const WorkerThread &) = delete;
    void operator =(const WorkerThread &) = delete;

private:
    void task_process();

    std::string name_;
    std::shared_ptr<std::thread> thread_;
    std::shared_ptr<TaskQueue> task_queue_;
};

namespace current_worker_thread {

/**
 * @brief 获取当前工作线程的TaskQueue
 *
 * @return TaskQueue的指针, 如果当前线程没有TaskQueue, 返回空指针
 */
std::shared_ptr<TaskQueue> get_task_queue();

/**
 * @brief 获取当前工作线程的名字
 *
 * @return 工作线程名
 *
 * @note 如果当前线程不是由WorkerThread创建的, 工作线程名可能为空
 */
const std::string &get_name();

}   // namespace this_thread

}   // namespace MiniUtils

#endif 
