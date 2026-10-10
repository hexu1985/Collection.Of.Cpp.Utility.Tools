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
    
    # 初始化读集合，加入服务器套接字
    reads = [serv_sock]
    
    while True:
        # 每轮循环复制一份读集合（因为 select 会修改传入的列表）
        cpy_reads = reads[:]
        
        # select 参数：读列表、写列表、异常列表、超时时间（5秒 + 5000微秒）
        try:
            fd_num, _, _ = select.select(cpy_reads, [], [], 5.005)
        except select.error:
            break
        except OSError:
            break
        
        # 超时（没有任何事件就绪）
        if fd_num == 0:
            continue
        
        # 遍历所有就绪的文件描述符
        for fd in cpy_reads:
            if fd == serv_sock:
                # 有新的连接请求
                try:
                    clnt_sock, clnt_addr = serv_sock.accept()
                    reads.append(clnt_sock)     # 加入读集合
                    print(f"connected client: {clnt_sock.fileno()} ")
                except socket.error:
                    error_handling("accept() error")
            else:
                # 有数据可读
                try:
                    data = fd.recv(BUF_SIZE)
                    if not data:
                        # 客户端关闭连接
                        reads.remove(fd)
                        fd.close()
                        print(f"closed client: {fd.fileno()} ")
                    else:
                        # 回声
                        fd.sendall(data)
                except socket.error:
                    # 出错时也移除该套接字
                    if fd in reads:
                        reads.remove(fd)
                    fd.close()
    
    serv_sock.close()

if __name__ == "__main__":
    main()
