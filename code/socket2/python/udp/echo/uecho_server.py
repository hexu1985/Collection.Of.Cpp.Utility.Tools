#!/usr/bin/env python3
import socket
import sys

BUF_SIZE = 30  # 对应 #define BUF_SIZE 30


def error_handling(message):
    print(message, file=sys.stderr)
    sys.exit(1)


def main():
    if len(sys.argv) != 2:
        print(f"Usage : {sys.argv[0]} <port>")
        sys.exit(1)

    # socket(PF_INET, SOCK_DGRAM, 0)
    #   PF_INET == AF_INET，SOCK_DGRAM 即 UDP
    serv_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    # Python 里 socket() 失败会直接抛 OSError，等价于检查 == -1

    try:
        # bind：等价于填充 sockaddr_in + bind()
        #   sin_addr = htonl(INADDR_ANY) -> "0.0.0.0"（或 "" 也行）
        #   sin_port = htons(atoi(argv[1])) -> int(port)
        serv_sock.bind(("0.0.0.0", int(sys.argv[1])))

        while True:
            # recvfrom 返回 (data, addr)
            message, clnt_adr = serv_sock.recvfrom(BUF_SIZE)
            # sendto 原样发回给来源地址
            serv_sock.sendto(message, clnt_adr)
    except OSError as e:
        error_handling(f"socket error: {e}")
    finally:
        serv_sock.close()


if __name__ == "__main__":
   main()
