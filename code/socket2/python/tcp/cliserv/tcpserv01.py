import sys
import socket
import os

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


def main():
    # 创建监听 socket
    listenfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # 绑定到 INADDR_ANY:SERV_PORT
    # 等价于 sin_addr.s_addr = htonl(INADDR_ANY), sin_port = htons(SERV_PORT)
    listenfd.bind(('', SERV_PORT))

    # 监听
    listenfd.listen(LISTENQ)

    while True:
        connfd, cliaddr = listenfd.accept()
        # cliaddr 是 (ip, port) 元组，对应 C 中的 cliaddr 结构

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
