import sys
import socket

SERV_PORT = 9877   # 对应 C 代码中的 SERV_PORT
MAXLINE = 4096     # 对应 MAXLINE

def str_cli(fp, sockfd):
    rf = sockfd.makefile('rb')
    for sendline in fp:
        sockfd.sendall(sendline.encode())
        recvline = rf.readline()          # 读到 '\n' 或 EOF
        if not recvline:
            print("str_cli: server terminated prematurely")
            sys.exit(1)
        sys.stdout.write(recvline.decode())
        sys.stdout.flush()

def main():
    if len(sys.argv) != 2:
        print("usage: tcpcli <IPaddress>")
        sys.exit(1)

    sockfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # 与 C 中 Inet_pton + sin_port 赋值等价
    servaddr = (sys.argv[1], SERV_PORT)

    sockfd.connect(servaddr)

    str_cli(sys.stdin, sockfd)

    sockfd.close()
    sys.exit(0)


if __name__ == "__main__":
    main()
