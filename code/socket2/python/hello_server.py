import socket
import sys

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
    
    try:
        # 接受客户端连接
        clnt_sock, clnt_addr = serv_sock.accept()
    except socket.error:
        error_handling("accept() error")
    
    message = "Hello World!"
    
    try:
        # 向客户端发送消息（注意：原C++代码发送了sizeof(message)即13字节，包含结尾的'\0'）
        clnt_sock.send(message.encode())
    except socket.error:
        error_handling("write() error")
    
    # 关闭套接字
    clnt_sock.close()
    serv_sock.close()

if __name__ == "__main__":
    main()
