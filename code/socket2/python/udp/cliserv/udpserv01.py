import sys
import socket

SERV_PORT = 9877    # 对应 C 代码中的 SERV_PORT
MAXLINE = 4096      # 对应 MAXLINE


def dg_echo(sockfd):
    """对应 C 中的 dg_echo：把收到的数据报原样回显给发送方"""
    while True:
        mesg, cliaddr = sockfd.recvfrom(MAXLINE)
        sockfd.sendto(mesg, cliaddr)


def main():
    # 创建 UDP socket
    sockfd = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # 绑定到 INADDR_ANY:SERV_PORT
    # 等价于 sin_addr.s_addr = htonl(INADDR_ANY), sin_port = htons(SERV_PORT)
    sockfd.bind(('', SERV_PORT))

    dg_echo(sockfd)


if __name__ == "__main__":
    main()
