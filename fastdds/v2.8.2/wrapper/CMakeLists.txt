cmake_minimum_required(VERSION 3.12.4)

if(NOT CMAKE_VERSION VERSION_LESS 3.0)
    cmake_policy(SET CMP0048 NEW)
endif()

project(fastdds-wrapper)

add_compile_options(-Werror=return-type)

# Find requirements
if(NOT fastcdr_FOUND)
    find_package(fastcdr REQUIRED)
endif()

if(NOT fastrtps_FOUND)
    find_package(fastrtps REQUIRED)
endif()

if(NOT tinyxml2_FOUND)
    find_package(tinyxml2 REQUIRED)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/fdds)
add_subdirectory(fdds)
add_subdirectory(examples/api)
