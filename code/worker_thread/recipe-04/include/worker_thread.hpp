#pragma once

#include <thread>
#include <string>
#include "task_queue.hpp"

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
    WorkerThread(const std::string& name="");

    /**
     * @brief 析构工作线程类对象
     */
    ~WorkerThread();

    WorkerThread(const WorkerThread&) = delete;
    WorkerThread& operator=(const WorkerThread&) = delete;

    WorkerThread(WorkerThread&&) = default;
    WorkerThread& operator=(WorkerThread&&) = default;

    /**
     * @brief 创建一个TaskQueue, 以及一个工作线程处理TaskQueue的任务
     */
    void Start();

    /**
     * @brief 停止工作线程, 会把TaskQueue中已有的任务都处理完, 才返回
     */
    void Stop();

    /**
     * @brief 工作线程是否运行
     *
     * @return 如果工作线程正在运行, 返回true; 否则返回false
     */
    bool IsRunning();

    /**
     * @brief 获取TaskQueue
     *
     * @return TaskQueue的指针
     */
    std::shared_ptr<TaskQueue> GetTaskQueue();

    /**
     * @brief 获取工作线程名
     *
     * @return 线程名
     */
    const std::string& GetThreadName() const;

    /**
     * @brief 获取当前工作线程的TaskQueue
     *
     * @return TaskQueue的指针, 如果当前线程没有TaskQueue, 返回空指针
     */
    static std::shared_ptr<TaskQueue> GetCurrentTaskQueue();

    /**
     * @brief 获取当前工作线程的名字
     *
     * @return 工作线程名
     *
     * @note 如果当前线程不是由WorkerThread创建的, 工作线程名可能为空
     */
    static const std::string& GetCurrentThreadName();

private:
    std::string name_;
    std::shared_ptr<std::thread> thread_;
    std::shared_ptr<TaskQueue> task_queue_;
};

