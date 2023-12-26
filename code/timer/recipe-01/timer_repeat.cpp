#include "timer.hpp"
#include <time.h>
#include <stdio.h>

void repeat();

void createTimer() {
    auto t = Timer(2, repeat);
    t.start();
}

void repeat() {
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "Now: %H:%M:%S", timeinfo);
    puts(buffer);
    createTimer();
}

int main() {
    createTimer();
    int c = getchar();
    (void) c;
    return 0;
}
