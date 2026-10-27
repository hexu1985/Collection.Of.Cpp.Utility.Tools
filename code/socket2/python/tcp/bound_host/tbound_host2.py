#!/usr/bin/env python3
import socket
import sys

BUF_SIZE = 30  # 对应 #define BUF_SIZE 30（本例其实没用到，保留以对齐）


def error_handling(message):
    print(message, file=sys.stderr)
    sys.exit(1)


def main():
    if len(sys.argv) != 3:
        print(f"Usage : {sys.argv[0]} <IP> <port>")
        sys.exit(1)

    host = sys.argv[1]
    port = int(sys.argv[2])

    msg1 = b"Hi!"
    msg2 = b"I'm another UDP host!"
    msg3 = b"Nice to meet you"

    # socket(PF_INET, SOCK_DGRAM, 0)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        # 对应 memset + sin_family + inet_addr + htons
        #   inet_addr(argv[1]) -> 字符串 IP 直接交给 sendto
        #   htons(atoi(argv[2])) -> 直接传 int
        serv_adr = (host, port)
        sock.connect(serv_adr)

        sock.send(msg1)
        sock.send(msg2)
        sock.send(msg3)
    finally:
        sock.close()

    return 0


if __name__ == "__main__":
    main()
