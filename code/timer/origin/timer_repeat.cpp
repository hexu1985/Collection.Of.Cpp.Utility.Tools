#include "timercpp.h"
#include <time.h>
#include <stdio.h>

using namespace std;

void repeat() {
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "Now: %H:%M:%S", timeinfo);
    puts(buffer);
}

int main() {
    Timer t;
    t.setInterval(repeat, 2000);

    int c = getchar();
    (void) c;
    return 0;
}
