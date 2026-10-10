import sys
import socket
import os
import signal
import errno

SERV_PORT = 9877    # 对应 C 代码中的 SERV_PORT
LISTENQ = 1024      # 对应 LISTENQ
MAXLINE = 4096      # 对应 MAXLINE


def str_echo(connfd):
    """对应 C 中的 str_echo：把收到的数据回显给客户端"""
    while True:
        data = connfd.recv(MAXLINE)
        if not data:
            break
        connfd.sendall(data)
    connfd.close()


def sig_chld(signum, frame):
    """对应 C 中的 sig_chld：回收已终止的子进程，避免僵尸进程"""
    while True:
        try:
            pid, status = os.waitpid(-1, os.WNOHANG)
        except ChildProcessError:
            # 没有子进程了（等价于 waitpid 返回 -1 且 errno == ECHILD）
            break
        if pid == 0:
            # 还有子进程在运行，但没有已终止的（等价于 waitpid 返回 0）
            break


def main():
    # 创建监听 socket
    listenfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # 绑定到 INADDR_ANY:SERV_PORT
    # 等价于 sin_addr.s_addr = htonl(INADDR_ANY), sin_port = htons(SERV_PORT)
    listenfd.bind(('', SERV_PORT))

    # 监听
    listenfd.listen(LISTENQ)

    # 注册 SIGCHLD 信号处理函数
    signal.signal(signal.SIGCHLD, sig_chld)

    while True:
        try:
            connfd, cliaddr = listenfd.accept()
        except InterruptedError:
            # 被信号中断，重新 accept（对应 C 中的 EINTR 处理）
            continue
        except OSError as e:
            if e.errno == errno.EINTR:
                continue
            print(f"accept error: {e}")
            sys.exit(1)

        pid = os.fork()
        if pid == 0:
            # 子进程
            listenfd.close()        # 关闭监听 socket
            str_echo(connfd)        # 处理请求
            os._exit(0)             # 对应 C 中的 exit(0)
        else:
            # 父进程
            connfd.close()          # 关闭已连接 socket


if __name__ == "__main__":
    main()
