#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <pthread.h>

static pid_t get_tid() {
    return syscall(SYS_gettid);
}

void* thread_func(void* arg) {
    pid_t tid = get_tid();
    pid_t pid = getpid();
    if (tid == pid) {
        printf("[Child] Unexpected: TID == PID (%d)\n", tid);
    } else {
        printf("[Child] TID = %d, PID = %d\n", tid, pid);
    }
    return NULL;
}

int main() {
    pid_t tid = get_tid();
    pid_t pid = getpid();
    if (tid == pid) {
        printf("[Main] TID = PID = %d\n", tid);
    }

    pthread_t thread;
    pthread_create(&thread, NULL, thread_func, NULL);
    pthread_join(thread, NULL);

    return 0;
}
