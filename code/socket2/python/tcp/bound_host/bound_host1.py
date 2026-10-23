import socket
import sys
import time

BUF_SIZE = 30  # 对应 #define BUF_SIZE 30

def error_handling(message):
    """错误处理函数"""
    print(message, file=sys.stderr)
    sys.exit(1)

def main():
    if len(sys.argv) != 2:
        print(f"Usage : {sys.argv[0]} <port>")
        sys.exit(1)
    
    port = int(sys.argv[1])

    try:
        # 创建TCP套接字
        serv_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    except socket.error:
        error_handling("socket() error")
    
    try:
        serv_sock.bind(("0.0.0.0", port))
    except socket.error:
        error_handling("bind() error")
    
    try:
        # 开始监听，最大等待队列为5
        serv_sock.listen(5)
    except socket.error:
        error_handling("listen() error")
    
    try:
        clnt_sock, clnt_addr = serv_sock.accept()
        print(f"Connected client ")
    except socket.error:
        error_handling("accept() error")
    
    try:
        i=0
        while True:
            time.sleep(5)  # sleep(5)：delay 5 sec
            message = clnt_sock.recv(BUF_SIZE)
            if not message:      # 收到空数据表示客户端关闭了连接
                break
            print(f"Message {i + 1}: {message.decode()} ")
            i+=1
    except socket.error:
        error_handling("read/write() error")
    
    clnt_sock.close()
    
    serv_sock.close()

if __name__ == "__main__":
    main()
