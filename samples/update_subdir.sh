#!/usr/bin/env bash

rm -f CMakeLists.txt

# 获取目录文件夹
for sub_dir in $(ls -l | grep '^d' | awk '{print $9}')
do
    echo "add_subdirectory(${sub_dir})" >> CMakeLists.txt
done
