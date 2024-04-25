#include "task_queue.hpp"
#include <thread>
#include <string>
#include <iostream>
#include <stdexcept>

void processor(TaskQueue &task_queue)
{
    while (true) {
        TaskQueue::InternalQueueType task_list;
        task_queue.PopTask(task_list);
        while (!task_list.empty()) {
            auto task = std::move(task_list.front());
            task_list.pop_front();
            try {
                task.Run();
            } catch (const std::runtime_error &e) {
                std::cout << e.what() << std::endl;
                return;
            }
        }
    }
}

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

class Foo {
    int n_;
    public:
    Foo(int n): n_(n) {}

    void print()
    {
        std::cout << "Foo::" << __func__ << "(" << n_++ << ")" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
};

void process_exit()
{
    std::cout << "task to stop process thread" << std::endl;
    throw std::runtime_error("stop process thread");
}

int main()
{
    Foo foo(1);
    std::shared_ptr<Foo> pfoo(new Foo(101));

    TaskQueue task_queue;
    std::thread proc_thread(processor, std::ref(task_queue));
    std::string str = "hello";
    for (int i = 0; i < 10; i++) {
        task_queue.PushTask(MakeTask(print_int, i));
        task_queue.PushTask(MakeTask(print_string, str));
        task_queue.PushTask(MakeTask(&Foo::print, &foo));
        task_queue.PushTask(MakeTask(&Foo::print, pfoo));
    }
    task_queue.PushTask(MakeTask(process_exit));
    proc_thread.join();
    return 0;
}

