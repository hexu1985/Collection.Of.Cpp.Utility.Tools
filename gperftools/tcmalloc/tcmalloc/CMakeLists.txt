cmake_minimum_required( VERSION 3.15 )

project ( tcmalloc_demo_project LANGUAGES CXX )

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(tcmalloc_installed_dir "$ENV{HOME}/local/gperftools/" CACHE STRING "tcmalloc installed directory") 
message(STATUS "tcmalloc_installed_dir: ${tcmalloc_installed_dir}")

#include_directories("${tcmalloc_installed_dir}/include")
link_directories("${tcmalloc_installed_dir}/lib")

message( STATUS "CMAKE_PROJECT_NAME = ${CMAKE_PROJECT_NAME}" )
message( STATUS "PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}" )

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

FOREACH (src_file ${SOURCES})
    get_filename_component(exe_file ${src_file} NAME_WLE)
    add_executable(${exe_file} ${src_file})
    target_link_libraries(${exe_file} tcmalloc pthread)
ENDFOREACH()
