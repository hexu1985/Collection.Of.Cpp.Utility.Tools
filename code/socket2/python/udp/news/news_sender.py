import sys
import socket
import time

TTL = 64            # 对应 C 代码中的 TTL
BUF_SIZE = 1024     # 对应 BUF_SIZE


def error_handling(message):
    print(message, file=sys.stderr)
    sys.exit(1)


def main():
    if len(sys.argv) != 3:
        print(f"Usage : {sys.argv[0]} <GroupIP> <PORT>")
        sys.exit(1)

    # 创建 UDP socket（对应 PF_INET, SOCK_DGRAM）
    send_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # 组播地址
    mul_addr = (sys.argv[1], int(sys.argv[2]))

    # 设置组播 TTL（对应 setsockopt IP_MULTICAST_TTL）
    send_sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, TTL)

    # 打开 news.txt
    try:
        fp = open("news.txt", "rb")
    except OSError:
        error_handling("fopen() error")

    # 组播发送
    while True:
        buf = fp.readline(BUF_SIZE)
        if not buf:
            break       # 对应 feof(fp) 判断
        send_sock.sendto(buf, mul_addr)
        time.sleep(1)

    fp.close()
    send_sock.close()


if __name__ == "__main__":
    main()
