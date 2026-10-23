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

    # 对应填充 serv_adr + inet_addr + htons
    #   inet_addr(argv[1]) -> 字符串 IP 交给 sendto 时自动解析
    #   htons(atoi(argv[2])) -> 直接传 int
    serv_adr = (host, port)

    try:
        while True:
            # fputs("Insert message(q to quit): ", stdout)
            # fgets(message, sizeof(message), stdin)
            try:
                message = input("Insert message(q to quit): ")
            except EOFError:            # Ctrl-D：等价于 fgets 返回 NULL
                break

            # strcmp(message, "q\n") / "Q\n" -> 退出
            #   注意 Python 的 input 已经去掉结尾换行，所以比 "q"/"Q"
            if message in ("q", "Q"):
                break

            # sendto(...)：注意 C 里发的是 str_len = strlen，不含 '\0'
            #   Python 中 .encode() 也不含 '\0'，语义一致
            data = message.encode()
            sock.sendto(data, serv_adr)

            # recvfrom(sock, message, BUF_SIZE, 0, ...)
            recv_data, from_adr = sock.recvfrom(BUF_SIZE)

            # printf("Message from server: %s", message)
            print(f"Message from server: {recv_data.decode()}")
    finally:
        sock.close()

    return 0


if __name__ == "__main__":
    main()
