import socket
import sys

def main():
    if len(sys.argv) != 3:
        sys.exit("usage: a.out <IPaddress> <port>")

    host = sys.argv[1]
    try:
        port = int(sys.argv[2])
    except ValueError:
        sys.exit(f"invalid port: {sys.argv[2]}")

    try:
        sockfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    except OSError as e:
        sys.exit(f"socket error: {e}")

    servaddr = (host, port)

    try:
        sockfd.connect(servaddr)
    except OSError as e:
        sys.exit(f"connect error: {e}")

    try:
        while True:
            data = sockfd.recv(1024)
            if not data:
                break
            sys.stdout.write(data.decode(errors="replace"))
            sys.stdout.flush()
    except OSError as e:
        sys.exit(f"read error: {e}")
    finally:
        sockfd.close()

if __name__ == "__main__":
    main()
