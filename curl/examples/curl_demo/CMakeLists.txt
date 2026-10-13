cmake_minimum_required(VERSION 3.10)
project(curl_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 libcurl
find_package(CURL REQUIRED)

# 可执行文件
add_executable(curl_demo main.cpp)

# 链接 libcurl
target_link_libraries(curl_demo PRIVATE CURL::libcurl)

# Windows 下需要额外链接一些库
if(WIN32)
    target_link_libraries(curl_demo PRIVATE ws2_32 crypt32 wldap32)
endif()
