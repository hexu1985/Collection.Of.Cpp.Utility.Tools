cmake_minimum_required(VERSION 3.12.4)

if(NOT CMAKE_VERSION VERSION_LESS 3.0)
    cmake_policy(SET CMP0048 NEW)
endif()

project(ClientServerTest)

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

# Set C++11
include(CheckCXXCompilerFlag)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG OR
        CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    check_cxx_compiler_flag(-std=c++11 SUPPORTS_CXX11)
    if(SUPPORTS_CXX11)
        add_compile_options(-std=c++11)
    else()
        message(FATAL_ERROR "Compiler doesn't support C++11")
    endif()
endif()

message(STATUS "Configuring DDSClientServerTest...")
#file(GLOB DDS_CLIENTSERVERTEST_SOURCES "src/*.cpp")
set(DDS_CLIENTSERVERTEST_SOURCES
    EprosimaRpcServer.cpp
    EprosimaRpcServer.hpp
    EprosimaRpcClient.cpp
    EprosimaRpcClient.hpp
    EprosimaRpcUtility.cpp
    EprosimaRpcUtility.hpp
    EprosimaPubWrapper.cpp
    EprosimaPubWrapper.hpp
    EprosimaSubWrapper.cpp
    EprosimaSubWrapper.hpp
    timer.cpp
    timer.hpp
    threadsafe_queue.hpp
    worker_thread.hpp
    gen/soa_on_dds_types.cxx
    gen/soa_on_dds_types.h
    gen/soa_on_dds_typesPubSubTypes.cxx
    gen/soa_on_dds_typesPubSubTypes.h
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/gen)

add_executable(calculate_server_test 
    calculate_server_test.cpp 
    EprosimaServer.cpp
    EprosimaServer.h
    gen/ClientServerTypes.cxx
    gen/ClientServerTypes.h
    ${DDS_CLIENTSERVERTEST_SOURCES})
target_link_libraries(calculate_server_test fastrtps fastcdr tinyxml2::tinyxml2)

add_executable(calculate_client_test 
    calculate_client_test.cpp 
    EprosimaClient.cpp
    EprosimaClient.h
    gen/ClientServerTypes.cxx
    gen/ClientServerTypes.h
    ${DDS_CLIENTSERVERTEST_SOURCES})
target_link_libraries(calculate_client_test fastrtps fastcdr tinyxml2::tinyxml2)

add_executable(calculate_client_test_async 
    calculate_client_test_async.cpp 
    EprosimaAsyncClient.cpp
    EprosimaAsyncClient.h
    gen/ClientServerTypes.cxx
    gen/ClientServerTypes.h
    ${DDS_CLIENTSERVERTEST_SOURCES})
target_link_libraries(calculate_client_test_async fastrtps fastcdr tinyxml2::tinyxml2)

add_executable(test_ClientServerTypes test_ClientServerTypes.cpp gen/ClientServerTypes.cxx)
target_link_libraries(test_ClientServerTypes fastrtps fastcdr tinyxml2::tinyxml2)

add_executable(calculate_client_test_pressure 
    calculate_client_test_pressure.cpp 
    EprosimaClient.cpp
    EprosimaClient.h
    EprosimaClientTest.cpp
    EprosimaClientTest.h
    gen/ClientServerTypes.cxx
    gen/ClientServerTypes.h
    ${DDS_CLIENTSERVERTEST_SOURCES})
target_link_libraries(calculate_client_test_pressure fastrtps fastcdr tinyxml2::tinyxml2)

add_executable(calculate_client_test_timed 
    calculate_client_test_timed.cpp 
    gen/ClientServerTypes.cxx
    gen/ClientServerTypes.h
    EprosimaClientTimed.cpp
    EprosimaClientTimed.h
    ${DDS_CLIENTSERVERTEST_SOURCES})
target_link_libraries(calculate_client_test_timed fastrtps fastcdr tinyxml2::tinyxml2)

add_executable(calculate_client_test_async_timed 
    calculate_client_test_async_timed.cpp 
    gen/ClientServerTypes.cxx
    gen/ClientServerTypes.h
    EprosimaAsyncClientTimed.cpp
    EprosimaAsyncClientTimed.h
    ${DDS_CLIENTSERVERTEST_SOURCES})
target_link_libraries(calculate_client_test_async_timed fastrtps fastcdr tinyxml2::tinyxml2)

add_executable(daytime_server_test 
    daytime_server_test.cpp 
    DaytimeServer.cpp
    DaytimeServer.h
    ${DDS_CLIENTSERVERTEST_SOURCES})
target_link_libraries(daytime_server_test fastrtps fastcdr tinyxml2::tinyxml2)
