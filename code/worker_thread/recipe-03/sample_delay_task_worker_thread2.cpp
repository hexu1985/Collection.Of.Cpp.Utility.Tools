#include <iostream>
#include <chrono>
#include <string>
#include "worker_thread.hpp"

void print_int(int i)
{
	std::cout << __func__ << "(" << i << ")" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

void print_string(std::string str)
{
	std::cout << __func__ << "(" << str << ")" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

void repush_task()
{
    auto task_queue = WorkerThread::GetCurrentTaskQueue();
    auto thread_name = WorkerThread::GetCurrentThreadName();
    std::cout << __func__ << " in thread " << thread_name << std::endl;
    if (task_queue) {
        std::cout << __func__ << " in thread " << thread_name << ", push another task" << std::endl;
        task_queue->PushTask(MakeTask(&print_string, thread_name), 100);
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
	for (int i = 0; i < 10; i++) {
		task_queue->PushTask(MakeTask(print_int, i), i*100);
		task_queue->PushTask(MakeTask(&repush_task), i*100+50);
	}

    std::cout << "main thread with sleep " << nsec << " seconds before stop thread " << mythread.GetThreadName() << std::endl;
    mythread.Stop(nsec*1000);
	return 0;
}

