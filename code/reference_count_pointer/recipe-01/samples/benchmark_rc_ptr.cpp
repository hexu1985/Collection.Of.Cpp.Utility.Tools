/** \example rc_ptr/benchmark_rc_ptr.cpp
 * This is an example of benchmark to RCPtr.
 */
#include <iostream>
#include "widget.hpp"

int main()
{
    Widget a("foo");
    Widget b("bar");
    Widget c(b);

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
