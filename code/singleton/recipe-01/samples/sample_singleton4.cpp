#include "singleton.hpp"
#include <iostream>

class Foo: public Singleton<Foo> {
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
    Foo &trace = Foo::getInstance();
    trace.step_1();
    trace.step_2();
    trace.step_3();

    // will return the existing instance
    Foo &trace2 = Foo::getInstance();
    trace2.step_1();
    trace2.step_2();
    trace2.step_3();

    // Foo foo;    // compile error
    // Foo foo3 = foo; // compile error

    return 0;
}

