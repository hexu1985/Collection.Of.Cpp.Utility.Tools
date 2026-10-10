import socket
import sys
import time

def main():
    if len(sys.argv) != 2:
        sys.exit("usage: a.out <port>")

    try:
        port = int(sys.argv[1])
    except ValueError:
        sys.exit(f"invalid port: {sys.argv[1]}")

    listenfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    listenfd.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    servaddr = ("0.0.0.0", port)  # INADDR_ANY
    listenfd.bind(servaddr)
    listenfd.listen(5)  # LISTENQ

    try:
        while True:
            connfd, _ = listenfd.accept()
            try:
                ticks = time.time()
                buff = time.ctime(ticks)[:24] + "\r\n"
                connfd.sendall(buff.encode())
            finally:
                connfd.close()
    except KeyboardInterrupt:
        pass
    finally:
        listenfd.close()

if __name__ == "__main__":
    main()
