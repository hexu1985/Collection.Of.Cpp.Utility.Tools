// test_malloc.c
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
int main() {
    clock_t start = clock();
    for (int i = 0; i < 100000000; i++) {
        void *p = malloc(1024);
        free(p);
    }
    printf("Time: %.2f sec\n", (double)(clock() - start) / CLOCKS_PER_SEC);
    return 0;
}
