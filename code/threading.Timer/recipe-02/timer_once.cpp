#include "Timer.hpp"
#include <stdio.h>

using threading::Timer;

void hello() {
    printf("hello, world\n");
}

int main() {
    auto t = Timer(3.0, hello);
    t.start();  // after 3 seconds, "hello, world" will be printed
    int c = getchar();
    (void) c;
    return 0;
}
