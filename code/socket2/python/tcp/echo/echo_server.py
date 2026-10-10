import socket
import sys

BUF_SIZE = 1024

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
        # 绑定地址和端口（INADDR_ANY 对应空字符串 ''）
        serv_addr = ('', int(sys.argv[1]))
        serv_sock.bind(serv_addr)
    except socket.error:
        error_handling("bind() error")
    
    try:
        # 开始监听，最大等待队列为5
        serv_sock.listen(5)
    except socket.error:
        error_handling("listen() error")
    
    # 最多服务5个客户端
    for i in range(5):
        try:
            clnt_sock, clnt_addr = serv_sock.accept()
            print(f"Connected client {i+1} ")
        except socket.error:
            error_handling("accept() error")
        
        try:
            # 回声循环：收到多少字节就回显多少字节，直到客户端关闭连接（recv返回空）
            while True:
                data = clnt_sock.recv(BUF_SIZE)
                if not data:      # 收到空数据表示客户端关闭了连接
                    break
                clnt_sock.send(data)   # 回显收到的数据
        except socket.error:
            error_handling("read/write() error")
        
        clnt_sock.close()
    
    serv_sock.close()

if __name__ == "__main__":
    main()
