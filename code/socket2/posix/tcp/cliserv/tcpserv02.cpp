#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <csignal>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const int SERV_PORT = 9877;   // 对应 C 代码中的 SERV_PORT
const int LISTENQ = 1024;     // 对应 LISTENQ
const int MAXLINE = 4096;     // 对应 MAXLINE

// 对应 C 中的 str_echo：把收到的数据回显给客户端
void str_echo(int connfd)
{
    char buf[MAXLINE];
    ssize_t n;

again:
    while ((n = read(connfd, buf, MAXLINE)) > 0) {
        // 把读到的 n 字节全部写回
        ssize_t nleft = n;
        char *ptr = buf;
        while (nleft > 0) {
            ssize_t nwritten = write(connfd, ptr, nleft);
            if (nwritten < 0) {
                if (errno == EINTR)
                    continue;
                std::perror("write");
                close(connfd);
                return;
            }
            nleft -= nwritten;
            ptr += nwritten;
        }
    }

    if (n < 0 && errno == EINTR)
        goto again;   // 被信号打断，重试
    else if (n < 0)
        std::perror("read");

    close(connfd);
}

// 对应 C 中的 sig_chld：回收已终止的子进程，避免僵尸进程
void sig_chld(int signo)
{
    pid_t pid;
    int stat;

    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
        // 可选：打印回收的子进程 pid
        // std::cout << "child " << pid << " terminated" << std::endl;
    }
    return;
}

int main()
{
    // 创建监听 socket
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        perror("socket");
        std::exit(1);
    }

    // 绑定到 INADDR_ANY:SERV_PORT
    struct sockaddr_in servaddr;
    std::memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    if (bind(listenfd, reinterpret_cast<struct sockaddr *>(&servaddr),
             sizeof(servaddr)) < 0) {
        perror("bind");
        std::exit(1);
    }

    // 监听
    if (listen(listenfd, LISTENQ) < 0) {
        perror("listen");
        std::exit(1);
    }

    // 注册 SIGCHLD 信号处理函数
    struct sigaction sa;
    sa.sa_handler = sig_chld;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;   // 不设置 SA_RESTART，让 accept 被中断时返回 EINTR
    if (sigaction(SIGCHLD, &sa, nullptr) < 0) {
        perror("sigaction");
        std::exit(1);
    }

    for (;;) {
        struct sockaddr_in cliaddr;
        socklen_t clilen = sizeof(cliaddr);

        int connfd = accept(listenfd,
                            reinterpret_cast<struct sockaddr *>(&cliaddr),
                            &clilen);
        if (connfd < 0) {
            if (errno == EINTR)
                continue;   // 被信号中断，重新 accept
            perror("accept");
            std::exit(1);
        }

        // 可选：打印客户端地址
        // char cliip[INET_ADDRSTRLEN];
        // inet_ntop(AF_INET, &cliaddr.sin_addr, cliip, sizeof(cliip));
        // std::cout << "connection from " << cliip << ":"
        //           << ntohs(cliaddr.sin_port) << std::endl;

        // fork 子进程处理连接
        pid_t pid = fork();
        if (pid == 0) {
            // 子进程
            close(listenfd);        // 关闭监听 socket
            str_echo(connfd);       // 处理请求
            std::exit(0);           // 对应 C 中的 exit(0)
        } else if (pid > 0) {
            // 父进程
            close(connfd);          // 关闭已连接 socket
        } else {
            perror("fork");
            close(connfd);
            std::exit(1);
        }
    }

    // 不会执行到这里
    close(listenfd);
    return 0;
}
