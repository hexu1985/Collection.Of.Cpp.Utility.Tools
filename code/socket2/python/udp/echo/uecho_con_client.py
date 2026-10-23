#!/usr/bin/env python3
import socket
import sys

BUF_SIZE = 30  # 对应 #define BUF_SIZE 30


def error_handling(message):
    print(message, file=sys.stderr)
    sys.exit(1)


def main():
    if len(sys.argv) != 3:
        print(f"Usage : {sys.argv[0]} <IP> <port>")
        sys.exit(1)

    host = sys.argv[1]
    port = int(sys.argv[2])

    # socket(PF_INET, SOCK_DGRAM, 0)
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    try:
        # connect(sock, &serv_adr, ...)
        #   对 UDP 而言：只设置默认对端，不发包、不做三次握手
        serv_adr = (host, port)
        sock.connect(serv_adr)

        while True:
            # fputs("Insert message(q to quit): ", stdout);
            # fgets(message, sizeof(message), stdin);
            try:
                message = input("Insert message(q to quit): ")
            except EOFError:            # 对应 fgets 返回 NULL
                break

            # !strcmp(message, "q\n") || !strcmp(message, "Q\n")
            #   input() 已去掉 '\n'，所以比 "q"/"Q"
            if message in ("q", "Q"):
                break

            # write(sock, message, strlen(message))
            #   已连接 UDP：用 send()，不需要地址
            sock.send(message.encode())

            # str_len = read(sock, message, sizeof(message)-1)
            #   sizeof(message)-1 == BUF_SIZE - 1 == 29
            #   已连接 UDP：用 recv()，不需要地址
            recv_data = sock.recv(BUF_SIZE - 1)

            # printf("Message from server: %s", message)
            print(f"Message from server: {recv_data.decode()}")
    finally:
        sock.close()

    return 0


if __name__ == "__main__":
    main()
