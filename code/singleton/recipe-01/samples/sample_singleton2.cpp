#include "singleton.hpp"
#include <iostream>

using namespace mini_util;

template <int N>
class Foo {
private:
    Foo() {
        std::cerr << "Foo<" << N << ">()\n"; 
    }

    friend class Singleton<Foo>;

public:
    ~Foo() {
        std::cerr << "~Foo<" << N << ">()\n"; 
    }

    void step_1() {
        std::cerr << "Foo<" << N << ">::" << "step_1\n";
    }

    void step_2() {
        std::cerr << "Foo<" << N << ">::" << "step_2\n";
    }

    void step_3() {
        std::cerr << "Foo<" << N << ">::" << "step_3\n";
    }
};

int main(int argc, char *argv[]) {
    Foo<1> &foo = Singleton<Foo<1>>::getInstance();
    foo.step_1();
    foo.step_2();
    foo.step_3();

    // will return the existing instance
    Foo<1> &foo2 = Singleton<Foo<1>>::getInstance();
    foo2.step_1();
    foo2.step_2();
    foo2.step_3();

    Foo<2> &foo3 = Singleton<Foo<2>>::getInstance();
    foo3.step_1();
    foo3.step_2();
    foo3.step_3();

    // will return the existing instance
    Foo<2> &foo4 = Singleton<Foo<2>>::getInstance();
    foo4.step_1();
    foo4.step_2();
    foo4.step_3();

    return 0;
}

