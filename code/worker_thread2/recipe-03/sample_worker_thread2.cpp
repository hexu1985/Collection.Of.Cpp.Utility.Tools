#include <iostream>
#include <chrono>
#include <functional>
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

class Functor {
	int n_;
public:
	Functor(int n): n_(n) {}

	void operator()()
	{
		std::cout << "Functor::" << __func__ << "(" << n_++ << ")" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
};

void join_task() {
    std::cout << "the last task complete" << std::endl;
}

int main()
{
	Foo foo(1);
    worker_thread mythread;

	for (int i = 0; i < 10; i++) {
        mythread.submit(Functor{i});
        mythread.submit(std::bind(print_int, i));
        mythread.submit(std::bind(print_string, std::string("hello")));
        mythread.submit(std::bind(&Foo::print, &foo));
	}
    Base base;
    Derived_A derived_a;
    Derived_B derived_b;

    Base *p1 = &base;
    Base *p2 = &derived_a;
    Base *p3 = &derived_b;

    mythread.submit(std::bind(&Base::print, p1));
    mythread.submit(std::bind(&Base::print, p2));
    mythread.submit(std::bind(&Base::print, p3));

    auto join_future = mythread.submit(&join_task);
    join_future.get();

	return 0;
}


