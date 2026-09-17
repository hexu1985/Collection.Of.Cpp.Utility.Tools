import socket
import sys

def error_handling(message):
    """错误处理函数"""
    print(message, file=sys.stderr)
    sys.exit(1)

def main():
    if len(sys.argv) != 3:
        print(f"Usage : {sys.argv[0]} <IP> <port>")
        sys.exit(1)
    
    try:
        # 创建TCP套接字
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    except socket.error as e:
        error_handling("socket() error")
    
    try:
        # 连接服务器
        serv_addr = (sys.argv[1], int(sys.argv[2]))
        sock.connect(serv_addr)
    except socket.error as e:
        error_handling("connect() error!")
    
    try:
        # 读取服务器发送的消息（最多30字节）
        message = sock.recv(30)
        if not message:
            error_handling("read() error!")
    except socket.error as e:
        error_handling("read() error!")
    
    print(f"Message from server: {message.decode()} ")
    
    # 关闭套接字
    sock.close()

if __name__ == "__main__":
    main()
