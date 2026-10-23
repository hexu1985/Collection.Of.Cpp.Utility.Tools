#!/usr/bin/env python3
import socket
import sys
import time

BUF_SIZE = 30  # 对应 #define BUF_SIZE 30


def error_handling(message):
    print(message, file=sys.stderr)
    sys.exit(1)


def main():
    if len(sys.argv) != 2:
        print(f"Usage : {sys.argv[0]} <port>")
        sys.exit(1)

    port = int(sys.argv[1])

    # socket(PF_INET, SOCK_DGRAM, 0)
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    try:
        # bind：my_adr.sin_addr = htonl(INADDR_ANY) -> "0.0.0.0"
        #       my_adr.sin_port = htons(atoi(argv[1])) -> int(port)
        sock.bind(("0.0.0.0", port))

        for i in range(3):
            time.sleep(5)  # sleep(5)：delay 5 sec

            # recvfrom(sock, message, BUF_SIZE, 0, &your_adr, &adr_sz)
            #   Python 直接返回 (data, addr)，不需要预分配缓冲区/长度变量
            message, your_adr = sock.recvfrom(BUF_SIZE)

            # printf("Message %d: %s \n", i+1, message)
            #   注意 C 的 %s 需要 NUL 结尾；Python bytes 直接 decode
            print(f"Message {i + 1}: {message.decode()} ")
    finally:
        sock.close()

    return 0


if __name__ == "__main__":
    main()
