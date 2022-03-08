#include <iostream>
#include <chrono>
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

class Base {
public:
    virtual void print() 
    {
        std::cout << "Base::print" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
};

class Derived_A : public Base {
public:
    virtual void print() 
    {
        std::cout << "Derived_A::print" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
};

class Derived_B : public Base {
public:
    virtual void print() 
    {
        std::cout << "Derived_B::print" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
};

int main()
{
	Foo foo(1);
    WorkerThread mythread("test");
    mythread.Start();

	auto task_queue = mythread.GetTaskQueue();
	for (int i = 0; i < 10; i++) {
		task_queue->PushTask(print_int, i);
		task_queue->PushTask(print_string, std::string("hello"));
		task_queue->PushTask(&Foo::print, &foo);
	}
    Base base;
    Derived_A derived_a;
    Derived_B derived_b;

    Base *p1 = &base;
    Base *p2 = &derived_a;
    Base *p3 = &derived_b;

    task_queue->PushTask(&Base::print, p1);
    task_queue->PushTask(&Base::print, p2);
    task_queue->PushTask(&Base::print, p3);

    mythread.Stop();
	return 0;
}

