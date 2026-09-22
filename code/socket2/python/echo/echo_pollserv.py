import socket
import select
import sys

BUF_SIZE = 100

def error_handling(message):
    """错误处理函数"""
    print(message, file=sys.stderr)
    sys.exit(1)

def main():
    if len(sys.argv) != 2:
        print(f"Usage : {sys.argv[0]} <port>")
        sys.exit(1)
    
    try:
        # 创建TCP套接字
        serv_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    except socket.error:
        error_handling("socket() error")
    
    try:
        # 绑定地址和端口
        serv_sock.bind(('', int(sys.argv[1])))
    except socket.error:
        error_handling("bind() error")
    
    try:
        # 开始监听
        serv_sock.listen(5)
    except socket.error:
        error_handling("listen() error")
    
    # 初始化 poll 对象，并注册服务器套接字的 POLLIN 事件
    poller = select.poll()
    poller.register(serv_sock, select.POLLIN)
    
    # fd -> socket 对象的映射，poll 返回的是文件描述符
    fd_to_sock = {serv_sock.fileno(): serv_sock}
    
    while True:
        # poll 超时时间单位是毫秒，5005 毫秒 ≈ 5.005 秒
        try:
            events = poller.poll(5005)
        except select.error:
            break
        except OSError:
            break
        
        # 超时（没有任何事件就绪）
        if not events:
            continue
        
        # 遍历所有就绪的事件（fd, event）
        for fd, event in events:
            sock = fd_to_sock.get(fd)
            if sock is None:
                continue
            
            if sock == serv_sock:
                # 有新的连接请求
                try:
                    clnt_sock, clnt_addr = serv_sock.accept()
                    poller.register(clnt_sock, select.POLLIN)
                    fd_to_sock[clnt_sock.fileno()] = clnt_sock
                    print(f"connected client: {clnt_sock.fileno()} ")
                except socket.error:
                    error_handling("accept() error")
            else:
                # 有数据可读（或对端关闭/出错）
                if event & (select.POLLERR | select.POLLHUP | select.POLLNVAL):
                    # 出错或挂断，直接关闭
                    poller.unregister(fd)
                    fd_to_sock.pop(fd, None)
                    sock.close()
                    print(f"closed client: {fd} ")
                    continue
                
                if event & select.POLLIN:
                    try:
                        data = sock.recv(BUF_SIZE)
                        if not data:
                            # 客户端关闭连接
                            poller.unregister(fd)
                            fd_to_sock.pop(fd, None)
                            sock.close()
                            print(f"closed client: {fd} ")
                        else:
                            # 回声
                            sock.sendall(data)
                    except socket.error:
                        # 出错时也移除该套接字
                        try:
                            poller.unregister(fd)
                        except (KeyError, OSError):
                            pass
                        fd_to_sock.pop(fd, None)
                        sock.close()
    
    serv_sock.close()

if __name__ == "__main__":
    main()
