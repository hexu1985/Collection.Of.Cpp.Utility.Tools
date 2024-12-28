#!/usr/bin/env python3

import requests
import os
import sys

# 指定服务器URL和要上传的文件路径
server_url = 'http://127.0.0.1:8000/'

testResponse = requests.get(server_url)
if testResponse.status_code == 200:
    print("与服务器的连接正常！")
else:
    print("无法连接到服务器！")

print("testResponse: {}".format(testResponse.text))

file_path = './test/file.bin'
with open(file_path, 'rb') as file:
    file_data = file.read()

response = requests.post(server_url, data=file_data)

if response.status_code == 200:
    print("文件上传成功！")
else:
    print("文件上传失败！")
