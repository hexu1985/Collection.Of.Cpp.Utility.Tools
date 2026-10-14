cmake_minimum_required( VERSION 3.15 )

project(socket2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message( STATUS "CMAKE_PROJECT_NAME = ${CMAKE_PROJECT_NAME}" )
message( STATUS "PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}" )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/unpsock )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty )
add_subdirectory(tcp)
add_subdirectory(udp)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

FOREACH (src_file ${SOURCES})
    get_filename_component(exe_file ${src_file} NAME_WLE)
    add_executable(${exe_file} ${src_file})
    #    target_link_libraries(${exe_file} fmt)
ENDFOREACH()

