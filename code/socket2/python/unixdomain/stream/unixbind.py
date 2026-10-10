import sys
import os
import socket

def main():
    if len(sys.argv) != 2:
        print("usage: unixbind <pathname>")
        sys.exit(1)

    sockfd = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

    try:
        os.unlink(sys.argv[1])  # OK if this fails
    except OSError:
        pass

    sockfd.bind(sys.argv[1])

    addr2 = sockfd.getsockname()
    print(f"bound name = {addr2}, returned len = {len(addr2) + 1}")
    # 注意：Python 的 getsockname() 返回的是路径字符串，长度这里用字符串长度+1近似 SUN_LEN

    sockfd.close()
    sys.exit(0)

if __name__ == "__main__":
    main()
