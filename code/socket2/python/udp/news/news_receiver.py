import sys
import socket
import struct

BUF_SIZE = 1024     # 对应 C 代码中的 BUF_SIZE


def error_handling(message):
    print(message, file=sys.stderr)
    sys.exit(1)


def main():
    if len(sys.argv) != 3:
        print(f"Usage : {sys.argv[0]} <GroupIP> <PORT>")
        sys.exit(1)

    group_ip = sys.argv[1]
    port = int(sys.argv[2])

    # 创建 UDP socket
    recv_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # 设置 SO_REUSEADDR（对应 setsockopt SO_REUSEADDR）
    recv_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    # 绑定到 INADDR_ANY:port（对应 htonl(INADDR_ANY) + bind）
    try:
        recv_sock.bind(('', port))
    except OSError:
        error_handling("bind() error")

    # 加入组播组（对应 IP_ADD_MEMBERSHIP）
    # struct ip_mreq { struct in_addr imr_multiaddr; struct in_addr imr_interface; };
    # 两个字段都是 4 字节网络字节序的 IPv4 地址
    mreq = struct.pack(
        '4s4s',
        socket.inet_aton(group_ip),      # imr_multiaddr
        socket.inet_aton('0.0.0.0')      # imr_interface = INADDR_ANY
    )
    recv_sock.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)

    # 接收循环
    while True:
        try:
            data, _ = recv_sock.recvfrom(BUF_SIZE - 1)
        except OSError:
            break
        sys.stdout.write(data.decode(errors='replace'))
        sys.stdout.flush()

    recv_sock.close()


if __name__ == "__main__":
    main()
