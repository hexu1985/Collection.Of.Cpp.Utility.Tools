import sys
import socket

UNIXSTR_PATH = "/tmp/unixstr"  # 对应 C 代码中的 UNIXSTR_PATH
MAXLINE = 4096


def str_cli(fp, sockfd):
    """从 fp 读取一行发送到 socket，并从 socket 读取一行写回 fp"""
    while True:
        sendline = fp.readline(MAXLINE)
        if not sendline:
            break  # EOF

        # Writen：Python 的 sendall 保证全部发送
        sockfd.sendall(sendline.encode())

        # Readline：从 socket 读取一行
        recvline = readline(sockfd, MAXLINE)
        if not recvline:
            print("str_cli: server terminated prematurely")
            sys.exit(1)

        sys.stdout.write(recvline.decode())
        sys.stdout.flush()


def readline(sockfd, maxlen):
    """模拟 C 中的 Readline：逐字节读取直到遇到 '\n' 或 EOF"""
    buf = bytearray()
    while len(buf) < maxlen:
        ch = sockfd.recv(1)
        if not ch:
            break
        buf += ch
        if ch == b'\n':
            break
    return bytes(buf)


def main():
    sockfd = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

    try:
        sockfd.connect(UNIXSTR_PATH)
    except OSError as e:
        print(f"connect error: {e}")
        sys.exit(1)

    str_cli(sys.stdin, sockfd)
    sockfd.close()
    sys.exit(0)


if __name__ == "__main__":
    main()
