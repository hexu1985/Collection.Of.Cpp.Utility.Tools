#include <thread>
#include <chrono>
#include <iostream>
#include "singleton.hpp"

using namespace mini_util;

class Foo {
public:
    Foo() {
        std::cerr << "Foo() begin\n"; 
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cerr << "Foo() end\n"; 
    }

    ~Foo() {
        std::cerr << "~Foo()\n"; 
    }

    void step_1() {
        std::cerr << "step_1\n";
    }

    void step_2() {
        std::cerr << "step_2\n";
    }

    void step_3() {
        std::cerr << "step_3\n";
    }
};

void func()
{
    Foo &foo = Singleton<Foo>::getInstance();
    foo.step_1();
    foo.step_2();
    foo.step_3();
}

int main(int argc, char *argv[]) {
    std::thread th(func);

    // will return the existing instance
    std::thread th2(func);

    th.join();
    th2.join();

    return 0;
}

