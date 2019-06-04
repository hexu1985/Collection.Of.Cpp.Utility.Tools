/** \example sample_thread2.cpp
 * This is an example of how to use the Thread class.
 */
#include <iostream>
#include <chrono>
#include <string>
#include "thread.hpp"

using namespace MiniUtils;

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
    auto task_queue = this_thread::get_task_queue();
    auto name = this_thread::get_name();
    std::cout << __func__ << " in thread " << name << std::endl;
    if (task_queue) {
        std::cout << __func__ << " in thread " << name << ", push another task" << std::endl;
        task_queue->push_task(&print_string, name);
    }
}

int main(int argc, char *argv[])
{
    int nsec = 3;
    if (argc == 2) {
        nsec = std::stoi(argv[1]);
    }

    Thread mythread("test");
    mythread.start();

	auto task_queue = mythread.get_task_queue();
	for (int i = 0; i < 10; i++) {
		task_queue->push_task(print_int, i);
		task_queue->push_task(&repush_task);
	}

    std::cout << "main thread with sleep " << nsec << " seconds before stop thread " << mythread.get_name() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(nsec));
    mythread.stop();
	return 0;
}

