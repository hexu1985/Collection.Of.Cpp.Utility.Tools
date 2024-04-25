#include <iostream>
#include <chrono>
#include <string>
#include "worker_thread.hpp"

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
        task_queue->PushTask(&repush_task, n-1);
    }
}

int main(int argc, char *argv[])
{
    int nsec = 3;
    if (argc == 2) {
        nsec = std::stoi(argv[1]);
    }

    WorkerThread mythread("test");
    mythread.Start();

    auto task_queue = mythread.GetTaskQueue();
    task_queue->PushTask(&repush_task, 10);

    std::cout << "main thread with sleep " << nsec << " seconds before stop thread " << mythread.GetThreadName() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(nsec));
    mythread.Stop();
    return 0;
}

