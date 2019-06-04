#ifndef GUL_MINI_UTILS_THREAD_INC
#define GUL_MINI_UTILS_THREAD_INC

#include <thread>
#include <string>

#include "task_queue.hpp"

namespace MiniUtils {

class Thread {
public:
    Thread(const std::string &name);
    ~Thread();

    void start();

    void stop();

    bool is_run();

    std::shared_ptr<TaskQueue> get_task_queue();

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

std::shared_ptr<TaskQueue> get_task_queue();

const std::string &get_name();

}   // namespace this_thread

}   // namespace MiniUtils

#endif 
