cmake_minimum_required(VERSION 3.16)
project(yaml_demo)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories( ${YAML_CPP_DIR}/include )
link_directories( ${YAML_CPP_DIR}/lib )

add_executable(demo1 demo1.cpp)
target_link_libraries(demo1 yaml-cpp)
