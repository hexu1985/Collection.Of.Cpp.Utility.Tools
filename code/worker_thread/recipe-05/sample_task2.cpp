#include "task.hpp"
#include <memory>
#include <thread>
#include <functional>
#include <string>
#include <vector>
#include <iostream>

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


int main()
{
    Foo foo(1);
    std::shared_ptr<Foo> pfoo(new Foo(101));

    std::vector<Task> task_list;
    std::string str = "hello";
    for (int i = 0; i < 10; i++) {
        task_list.push_back(Task(std::bind(print_int, i)));
        task_list.push_back(Task(std::bind(print_string, str)));
        task_list.push_back(Task(std::bind(&Foo::print, &foo)));
        task_list.push_back(Task(std::bind(&Foo::print, pfoo)));
    }

    for (auto& task : task_list) {
        task.Run();
    }
    return 0;
}


