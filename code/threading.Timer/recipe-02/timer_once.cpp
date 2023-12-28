#include "timer.hpp"
#include <stdio.h>

void hello() {
    printf("hello, world\n");
}

int main() {
    auto t = Timer(3, hello);
    t.start();  // after 3 seconds, "hello, world" will be printed
    int c = getchar();
    (void) c;
    return 0;
}
