import socket
import select
import sys
import errno

BUF_SIZE = 4
EPOLL_SIZE = 50


def set_nonblocking(sock):
    """将 socket 设为非阻塞模式"""
    sock.setblocking(False)


def main():
    if len(sys.argv) != 2:
        print(f"Usage : {sys.argv[0]} <port>")
        sys.exit(1)

    serv_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    serv_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    serv_sock.bind(("", int(sys.argv[1])))
    serv_sock.listen(5)

    epfd = select.epoll()
    fd_to_sock = {serv_sock.fileno(): serv_sock}

    # 监听 socket 设为非阻塞
    set_nonblocking(serv_sock)
    epfd.register(serv_sock.fileno(), select.EPOLLIN)

    try:
        while True:
            events = epfd.poll(EPOLL_SIZE, -1)
            if not events:
                continue

            print("return epoll_wait")
            for fd, event in events:
                if fd == serv_sock.fileno():
                    # 有新客户端连接
                    clnt_sock, clnt_adr = serv_sock.accept()
                    set_nonblocking(clnt_sock)
                    # 注意：注册时加上 EPOLLET（边缘触发）
                    epfd.register(clnt_sock.fileno(),
                                  select.EPOLLIN | select.EPOLLET)
                    fd_to_sock[clnt_sock.fileno()] = clnt_sock
                    print(f"connected client: {clnt_sock.fileno()}")
                else:
                    clnt_sock = fd_to_sock.get(fd)
                    if clnt_sock is None:
                        continue

                    # 边缘触发模式下，必须一直读直到 EAGAIN
                    while True:
                        try:
                            data = clnt_sock.recv(BUF_SIZE)
                        except BlockingIOError:
                            # 对应 C 里的 errno == EAGAIN，数据读完了
                            break
                        except ConnectionResetError:
                            data = b""

                        if not data:
                            # str_len == 0，客户端关闭
                            epfd.unregister(fd)
                            clnt_sock.close()
                            del fd_to_sock[fd]
                            print(f"closed client: {fd}")
                            break
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
