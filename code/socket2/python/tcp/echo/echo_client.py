import socket
import sys

BUF_SIZE = 1024

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
    except socket.error:
        error_handling("socket() error")
    
    try:
        # 连接服务器
        serv_addr = (sys.argv[1], int(sys.argv[2]))
        sock.connect(serv_addr)
        print("Connected...........")
    except socket.error:
        error_handling("connect() error!")
    
    # 循环收发消息（回声）
    while True:
        try:
            message = input("Input message(Q to quit): ")
        except EOFError:
            break
        
        if message == 'q' or message == 'Q':
            break

        if len(message) == 0:
            continue
        
        message += '\n'

        try:
            # 发送消息（注意：原C代码用strlen，不发送结尾的'\0'，所以这里也不加换行）
            sock.send(message.encode())
            
            # 接收服务器回显的消息
            data = sock.recv(BUF_SIZE - 1)
            if not data:
                break
            print(f"Message from server: {data.decode()}", end='')
        except socket.error:
            error_handling("send/recv() error")
    
    sock.close()

if __name__ == "__main__":
    main()
