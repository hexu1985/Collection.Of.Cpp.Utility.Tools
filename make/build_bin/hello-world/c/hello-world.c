#include <stdio.h>
#include <stdlib.h>

char *say_hello() { return "Hello, world!"; }

int main() {
    printf("%s\n", say_hello());
    return EXIT_SUCCESS;
}
