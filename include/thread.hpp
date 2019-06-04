#ifndef GUL_MINI_UTILS_THREAD_INC
#define GUL_MINI_UTILS_THREAD_INC

#include <thread>
#include <string>

#include "task_queue.hpp"

namespace MiniUtils {

/**
 * @brief 基于TaskQueue的线程类
 */
class Thread {
public:
    /**
     * @brief 构造一个线程类对象
     *
     * @param name 线程名
     */
    Thread(const std::string &name);

    /**
     * @brief 析构线程类对象
     */
    ~Thread();

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
     * @brief 获取线程名
     *
     * @return 线程名
     */
    const std::string &get_name() const;

private:
    Thread(const Thread &) = delete;
    void operator =(const Thread &) = delete;

private:
    void task_process();

    std::string name_;
    std::shared_ptr<std::thread> thread_;
    std::shared_ptr<TaskQueue> task_queue_;
};

namespace this_thread {

/**
 * @brief 获取当前线程的TaskQueue
 *
 * @return TaskQueue的指针, 如果当前线程没有TaskQueue, 返回空指针
 */
std::shared_ptr<TaskQueue> get_task_queue();

/**
 * @brief 获取当前线程的名字
 *
 * @return 线程名
 */
const std::string &get_name();

}   // namespace this_thread

}   // namespace MiniUtils

#endif 
