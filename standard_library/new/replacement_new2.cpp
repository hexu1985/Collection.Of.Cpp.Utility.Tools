#include<iostream>
using namespace std;

struct Foo {
    int a;
    int b;

    Foo() {
        a = 1;
        b = 2;
    }
};

int main() {
    char buf[sizeof(Foo)];
    Foo* p = (Foo*) buf;
    new(p) Foo;
    return 0;
}
