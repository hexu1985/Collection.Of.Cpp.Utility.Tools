/** \example rci_ptr/sample_rci_ptr.cpp
 * This is an example of how to use RCIPtr.
 */
#include <iostream>

#include "RCIPtr.hpp"
#include "widget.hpp"

using namespace mini_util;

class RCWidget {
public:
    RCWidget(const std::string &someThing): widget_(new Widget(someThing))
    {
    }

    void setValue(const std::string &value)
    {
        widget_->setValue(value);
    }

    void doThis() { widget_->doThis(); }
    const std::string &showThat() const { return widget_->showThat(); }

private:
    RCIPtr<Widget> widget_;
};

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
