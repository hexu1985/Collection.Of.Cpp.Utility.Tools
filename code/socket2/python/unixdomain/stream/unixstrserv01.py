import os
import sys
import socket
import signal
import errno

UNIXSTR_PATH = "/tmp/unixstr"   # 对应 C 代码中的 UNIXSTR_PATH
LISTENQ = 1024                  # 对应 LISTENQ
MAXLINE = 4096


def str_echo(connfd):
    """对应 C 中的 str_echo：把收到的数据回显给客户端"""
    while True:
        data = connfd.recv(MAXLINE)
        if not data:
            break
        connfd.sendall(data)
    connfd.close()


def sig_chld(signum, frame):
    """对应 C 中的 sig_chld：回收子进程，避免僵尸进程"""
    while True:
        try:
            pid, status = os.waitpid(-1, os.WNOHANG)
        except ChildProcessError:
            break
        if pid == 0:
            break


def main():
    # 创建监听 socket
    listenfd = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

    # 删除可能存在的旧 socket 文件
    try:
        os.unlink(UNIXSTR_PATH)
    except OSError:
        pass

    # 绑定
    listenfd.bind(UNIXSTR_PATH)

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

        # fork 子进程处理连接
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
