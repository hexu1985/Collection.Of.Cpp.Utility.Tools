#include "timer.hpp"
#include <stdio.h>

using namespace std;

void hello() {
    printf("hello, world\n");
}

int main() {
    Timer t;
    t.setTimeout(hello, chrono::milliseconds(3000));    // after 3 seconds, "hello, world" will be printed
    int c = getchar();
    (void) c;
    return 0;
}
