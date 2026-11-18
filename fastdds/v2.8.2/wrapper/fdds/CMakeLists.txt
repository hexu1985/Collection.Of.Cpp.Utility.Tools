cmake_minimum_required( VERSION 3.15 )

project(fdds-lib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message( STATUS "CMAKE_PROJECT_NAME = ${CMAKE_PROJECT_NAME}" )
message( STATUS "PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}" )

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

add_library(fdds 
    SHARED 
    ${SOURCES}
    )
target_include_directories(fdds PUBLIC .)
target_link_libraries(fdds fastrtps fastcdr tinyxml2::tinyxml2)
