#include "Singleton.hpp"
#include <iostream>

using namespace mini_utils;

class Foo {
private:
    Foo() {
        std::cerr << "Foo()\n"; 
    }

    friend class Singleton<Foo>;

public:
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

int main(int argc, char *argv[]) {
    Foo &foo = Singleton<Foo>::getInstance();
    foo.step_1();
    foo.step_2();
    foo.step_3();

    // will return the existing instance
    Foo &foo2 = Singleton<Foo>::getInstance();
    foo2.step_1();
    foo2.step_2();
    foo2.step_3();

    Foo foo3 = foo;

    return 0;
}

