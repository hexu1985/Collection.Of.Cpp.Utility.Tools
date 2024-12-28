#!/usr/bin/env python3

from http.server import BaseHTTPRequestHandler, HTTPServer
import time
import json

data = {'result': 'this is a test'}

# 创建自定义的请求处理类
class FileUploadHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header('Content-type', 'application/json')
        self.end_headers()
        self.wfile.write(json.dumps(data).encode())

    def do_POST(self):
        start_time = time.time()

        content_length = int(self.headers['Content-Length'])
        # 读取客户端发送的二进制文件数据
        file_data = self.rfile.read(content_length)

        print('headers', self.headers)
        print("do post:", self.path, self.client_address, file_data)

        # 在这里可以对接收到的文件数据进行处理，例如保存到磁盘
        with open('uploaded_file.bin', 'wb') as file:
            file.write(file_data)

        self.send_response(200)
        self.end_headers()
        self.wfile.write(b'File uploaded successfully.')

        end_time = time.time()
        time_elapsed_ms = int((end_time - start_time) * 1000)
        print(f"Update in {time_elapsed_ms} ms")


# 启动服务器
def run_server():
    server_address = ('0.0.0.0', 8000)  # 可以根据需要修改端口号
    httpd = HTTPServer(server_address, FileUploadHandler)
    print('Server running on port 8000...')
    httpd.serve_forever()


# 运行服务器
run_server()
