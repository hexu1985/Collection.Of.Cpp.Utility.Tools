#include <iostream>
#include "rc_widget.hpp"

int main()
{
    RCWidget a("foo");
    RCWidget b("bar");
    RCWidget c(b);

    std::cout << "a-------------" << std::endl;
    a.doThis();
    a.showThat();

    std::cout << "b-------------" << std::endl;
    b.doThis();
    b.showThat();

    std::cout << "c-------------" << std::endl;
    c.doThis();
    c.showThat();

    c = a;

    std::cout << "c-------------" << std::endl;
    c.doThis();
    c.showThat();

    return 0;
}
