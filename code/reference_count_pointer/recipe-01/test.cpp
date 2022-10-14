#include <iostream>

#include "string.hpp"
#include "rc_widget.hpp"

/******************************************************************************
* Functions to perform VERY simple test of the above.                         *
******************************************************************************/  
void testRCPtr()
{
    String s1 = "String with no changes.";
    String s2(s1);

    s2[12] = s2[13] = ' ';

    std::cout << s1 << '\n';      // prints "String with no changes."
    std::cout << s2 << '\n';      // prints "String with    changes."
}

void testRCIPtr()
{
    RCWidget w1(10);
    RCWidget w2(w1);

    w2.DoThis();

    std::cout << w1.ShowThat() << '\n';       // prints 10
    std::cout << w2.ShowThat() << '\n';       // prints -1
}

int main()
{
    testRCPtr();
    testRCIPtr();

    return 0;
}
