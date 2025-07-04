#!/usr/bin/bash

cd $(dirname ${BASH_SOURCE[0]})

set -x

# 查看帮助
./myprogram --help

# 正常使用
./myprogram --file data.txt --debug --vec 1 2 3

# 错误处理（缺少必需参数或类型错误）
./myprogram --file 123 --debug=invalid

