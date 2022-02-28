#include <iostream>
#include <chrono>
#include <string>
#include "worker_thread.hpp"

using namespace mini_util;

void repush_task(int n)
{
    auto task_queue = WorkerThread::GetCurrentTaskQueue();
    auto thread_name = WorkerThread::GetCurrentThreadName();
    std::cout << __func__ << " in thread " << thread_name << std::endl;
    std::cout << "n: " << n << std::endl;;
    if (n <= 0)
      return;
    if (task_queue) {
        std::cout << __func__ << " in thread " << thread_name << ", push another task" << std::endl;
        task_queue->PushTask(MakeTask(&repush_task, n-1), n*100);
    }
}

int main(int argc, char *argv[])
{
    int nsec = 5;
    if (argc == 2) {
        nsec = std::stoi(argv[1]);
    }

    WorkerThread mythread("test");
    mythread.Start();

    auto task_queue = mythread.GetTaskQueue();
    task_queue->PushTask(MakeTask(&repush_task, 10));

    std::cout << "main thread with sleep " << nsec << " seconds before stop thread " << mythread.GetThreadName() << std::endl;
    mythread.Stop(nsec*1000);
    return 0;
}

