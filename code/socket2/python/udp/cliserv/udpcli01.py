import sys
import socket

SERV_PORT = 9877    # 对应 C 代码中的 SERV_PORT
MAXLINE = 4096      # 对应 MAXLINE

def dg_cli(fp, sockfd, servaddr):
    """对应 C 中的 dg_cli：从 fp 读一行发给服务器，收一个数据报写到 stdout"""
    while True:
        sendline = fp.readline(MAXLINE)
        if not sendline:
            break  # EOF

        sockfd.sendto(sendline.encode(), servaddr)

        recvline, _ = sockfd.recvfrom(MAXLINE)
        sys.stdout.write(recvline.decode())
        sys.stdout.flush()

def main():
    if len(sys.argv) != 2:
        print("usage: udpcli <IPaddress>")
        sys.exit(1)

    # 服务器地址（对应 bzero + sin_family + sin_port + Inet_pton）
    servaddr = (sys.argv[1], SERV_PORT)

    # 创建 UDP socket
    sockfd = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    dg_cli(sys.stdin, sockfd, servaddr)

    sockfd.close()
    sys.exit(0)


if __name__ == "__main__":
    main()
