cmake_minimum_required(VERSION 3.16)
project(yaml_demo)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories( ${YAML_CPP_DIR}/include )
link_directories( ${YAML_CPP_DIR}/lib )

add_executable(demo1 demo1.cpp)
target_link_libraries(demo1 yaml-cpp)

add_executable(demo2 demo2.cpp)
target_link_libraries(demo2 yaml-cpp)

add_executable(demo3 demo3.cpp)
target_link_libraries(demo3 yaml-cpp)

add_executable(load_config load_config.cpp)
target_link_libraries(load_config yaml-cpp)

add_executable(detect_scalar_type detect_scalar_type.cpp)
target_link_libraries(detect_scalar_type yaml-cpp)

add_executable(null_as_string null_as_string.cpp)
target_link_libraries(null_as_string yaml-cpp)

add_executable(detect_node_type detect_node_type.cpp)
target_link_libraries(detect_node_type yaml-cpp)

add_executable(traversal_sub_key traversal_sub_key.cpp)
target_link_libraries(traversal_sub_key yaml-cpp)

add_executable(copy_node copy_node.cpp)
target_link_libraries(copy_node yaml-cpp)

add_executable(save_file save_file.cpp)
target_link_libraries(save_file yaml-cpp)

add_executable(scalar_array scalar_array.cpp)
target_link_libraries(scalar_array yaml-cpp)
