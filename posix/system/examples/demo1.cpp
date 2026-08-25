#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int main() {
    // 1. 执行简单命令
    int status = system("ls -l /nosuch");

    if (status == -1) {
        // system() 调用本身失败（fork失败或shell无法启动）
        fprintf(stderr, "system() failed: %s\n", strerror(errno));
        return 1;
    }

    // 2. 解析退出状态
    if (WIFEXITED(status)) {
        int exit_code = WEXITSTATUS(status);
        printf("命令正常执行，退出码: %d\n", exit_code);
        if (exit_code != 0) {
            fprintf(stderr, "命令执行失败（非零退出码）\n");
        }
    } else if (WIFSIGNALED(status)) {
        int sig = WTERMSIG(status);
        fprintf(stderr, "命令被信号 %d 终止 (%s)\n", sig, strsignal(sig));
    } else if (WIFSTOPPED(status)) {
        // 一般很少发生
        fprintf(stderr, "命令被暂停\n");
    }

    return 0;
}
