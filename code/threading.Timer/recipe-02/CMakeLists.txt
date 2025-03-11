cmake_minimum_required( VERSION 3.15 )

project ( timer_demo_project )

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message( STATUS "CMAKE_PROJECT_NAME = ${CMAKE_PROJECT_NAME}" )
message( STATUS "PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}" )

# 启用 AddressSanitizer
add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
file(GLOB PROGRAMS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/examples/*.cpp)

FOREACH (src_file ${PROGRAMS_SOURCES})
    get_filename_component(exe_file ${src_file} NAME_WLE)
    add_executable(${exe_file} ${src_file} ${SOURCES})
    target_include_directories(${exe_file} PRIVATE src examples)
    target_link_libraries(${exe_file} pthread)
ENDFOREACH()
