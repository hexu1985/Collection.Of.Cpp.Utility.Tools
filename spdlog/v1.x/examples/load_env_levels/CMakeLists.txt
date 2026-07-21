cmake_minimum_required( VERSION 3.15 )

project ( spdlog_demo_project LANGUAGES CXX )

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)

message( STATUS "CMAKE_PROJECT_NAME = ${CMAKE_PROJECT_NAME}" )
message( STATUS "PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}" )

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

FOREACH (src_file ${SOURCES})
    get_filename_component(exe_file ${src_file} NAME_WLE)
    add_executable(${exe_file} ${src_file})
    target_link_libraries(${exe_file} spdlog::spdlog)
ENDFOREACH()
