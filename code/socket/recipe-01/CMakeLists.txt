# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
project(wrapsocket LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package( gflags REQUIRED )

if ( gflags_FOUND )
    message(STATUS "Found gflags")
else()
    message(STATUS "Not found gflags")
endif ()

# generate a library from sources
add_library(wrapsocket 
    STATIC 
    SocketException.hpp 
    SocketException.cpp
    Socket.hpp 
    Socket.cpp
    SocketUtility.hpp 
    SocketUtility.cpp
    )

add_executable(streamer streamer.cpp)
target_link_libraries(streamer wrapsocket gflags)

add_executable(streamer6 streamer6.cpp)
target_link_libraries(streamer6 wrapsocket gflags)

add_executable(blocks blocks.cpp)
target_link_libraries(blocks wrapsocket gflags)

add_executable(tcp_sixteen tcp_sixteen.cpp)
target_link_libraries(tcp_sixteen wrapsocket gflags)

add_executable(byteorder byteorder.cpp)
target_link_libraries(byteorder wrapsocket gflags)

add_executable(local_machine_info local_machine_info.cpp)
target_link_libraries(local_machine_info wrapsocket gflags)
