#include <iostream>
#include <chrono>
#include <string>
#include "worker_thread.hpp"

using namespace mini_util;

void repush_task(int n)
{
    auto task_queue = current_worker_thread::get_task_queue();
    auto name = current_worker_thread::get_name();
    std::cout << __func__ << " in thread " << name << std::endl;
    std::cout << "n: " << n << std::endl;;
    if (n <= 0)
      return;
    if (task_queue) {
        std::cout << __func__ << " in thread " << name << ", push another task" << std::endl;
        task_queue->pushTask(&repush_task, n-1);
    }
}

int main(int argc, char *argv[])
{
    int nsec = 3;
    if (argc == 2) {
        nsec = std::stoi(argv[1]);
    }

    WorkerThread mythread("test");
    mythread.start();

    auto task_queue = mythread.getTaskQueue();
    task_queue->pushTask(&repush_task, 10);

    std::cout << "main thread with sleep " << nsec << " seconds before stop thread " << mythread.getName() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(nsec));
    mythread.stop();
    return 0;
}

