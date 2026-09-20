import socket
import select
import sys

BUF_SIZE = 100
EPOLL_SIZE = 50


def main():
    if len(sys.argv) != 2:
        print(f"Usage : {sys.argv[0]} <port>")
        sys.exit(1)

    serv_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    serv_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    serv_sock.bind(("", int(sys.argv[1])))
    serv_sock.listen(5)

    epfd = select.epoll()
    epfd.register(serv_sock.fileno(), select.EPOLLIN)

    # 用 fd -> socket 对象 的映射，方便后续操作
    fd_to_sock = {serv_sock.fileno(): serv_sock}

    try:
        while True:
            events = epfd.poll(EPOLL_SIZE, -1)  # -1 表示阻塞等待
            if not events:
                continue

            for fd, event in events:
                if fd == serv_sock.fileno():
                    # 有新客户端连接
                    clnt_sock, clnt_adr = serv_sock.accept()
                    epfd.register(clnt_sock.fileno(), select.EPOLLIN)
                    fd_to_sock[clnt_sock.fileno()] = clnt_sock
                    print(f"connected client: {clnt_sock.fileno()}")
                else:
                    clnt_sock = fd_to_sock.get(fd)
                    if clnt_sock is None:
                        continue

                    try:
                        data = clnt_sock.recv(BUF_SIZE)
                    except ConnectionResetError:
                        data = b""

                    if not data:
                        # 客户端关闭连接
                        epfd.unregister(fd)
                        clnt_sock.close()
                        del fd_to_sock[fd]
                        print(f"closed client: {fd}")
                    else:
                        # echo 回去
                        clnt_sock.sendall(data)
    except KeyboardInterrupt:
        pass
    finally:
        serv_sock.close()
        epfd.close()


if __name__ == "__main__":
    main()
