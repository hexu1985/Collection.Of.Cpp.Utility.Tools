#include "socket.hpp"
#include "wrapsignal.hpp"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <unistd.h>

using namespace unpsock;

const char *UNIXSTR_PATH = "/tmp/unixstr";  // 对应 C 代码中的 UNIXSTR_PATH
const int LISTENQ = 1024;     // 对应 LISTENQ
const int MAXLINE = 4096;     // 对应 MAXLINE

// 对应 C 中的 str_echo：把收到的数据回显给客户端
void str_echo(Socket& sock)
{
    while (true) {
        auto data = sock.recv(MAXLINE);
        if (data.empty())
            break;
        sock.sendall(data);
    }

    sock.close();
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
    Socket listen_sock(Family::UNIX, AddrType::STREAM);

    unlink(UNIXSTR_PATH);

    listen_sock.bind(Address::from_unix(UNIXSTR_PATH));
    listen_sock.listen(LISTENQ);

    Signal(SIGCHLD, &sig_chld);

    for (;;) {
        auto [conn_sock, cliaddr] = listen_sock.accept();

        // fork 子进程处理连接
        pid_t pid = fork();
        if (pid == 0) {
            // 子进程
            listen_sock.close();
            str_echo(conn_sock);       // 处理请求
            std::exit(0);           // 对应 C 中的 exit(0)
        } else if (pid > 0) {
            // 父进程
            conn_sock.close();
        } else {
            perror("fork");
            std::exit(1);
        }
    }

    // 不会执行到这里
    listen_sock.close();
    return 0;
}
