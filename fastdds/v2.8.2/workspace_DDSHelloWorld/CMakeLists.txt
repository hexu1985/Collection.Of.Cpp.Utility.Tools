cmake_minimum_required(VERSION 3.12.4)

if(NOT CMAKE_VERSION VERSION_LESS 3.0)
    cmake_policy(SET CMP0048 NEW)
endif()

project(DDSHelloWorld)

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

### test file
set(TEST_FILES my_profiles.xml)

foreach(test_file ${TEST_FILES})
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${test_file}
        #COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/src/${test_file}
                ${CMAKE_CURRENT_BINARY_DIR}/${test_file}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/${test_file}
        COMMENT "Copying test test_file ${test_file}"
    )
endforeach()

add_custom_target(copy_test_files ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/my_profiles.xml
)

message(STATUS "Configuring HelloWorld publisher/subscriber example...")
file(GLOB DDS_HELLOWORLD_SOURCES_CXX "src/*.cxx")

add_executable(DDSHelloWorldPublisher src/HelloWorldPublisher.cpp ${DDS_HELLOWORLD_SOURCES_CXX})
target_link_libraries(DDSHelloWorldPublisher fastrtps fastcdr tinyxml2::tinyxml2)

add_executable(DDSHelloWorldSubscriber src/HelloWorldSubscriber.cpp ${DDS_HELLOWORLD_SOURCES_CXX})
target_link_libraries(DDSHelloWorldSubscriber fastrtps fastcdr tinyxml2::tinyxml2)

add_executable(DDSHelloWorldSubscriberWithLog 
    src/HelloWorldSubscriberWithLog.cpp 
    src/ParticipantListener.cpp
    ${DDS_HELLOWORLD_SOURCES_CXX})
target_link_libraries(DDSHelloWorldSubscriberWithLog fastrtps fastcdr tinyxml2::tinyxml2)

add_executable(DDSHelloWorldPublisherWithLog 
    src/HelloWorldPublisherWithLog.cpp 
    src/ParticipantListener.cpp
    ${DDS_HELLOWORLD_SOURCES_CXX})
target_link_libraries(DDSHelloWorldPublisherWithLog fastrtps fastcdr tinyxml2::tinyxml2)

add_executable(DDSPrintDataReaderQos src/printDataReaderQos.cpp src/test_printDataReaderQos.cpp ${DDS_HELLOWORLD_SOURCES_CXX})
target_link_libraries(DDSPrintDataReaderQos fastrtps fastcdr tinyxml2::tinyxml2)

add_executable(DDSPrintDataWriterQos src/printDataWriterQos.cpp src/test_printDataWriterQos.cpp ${DDS_HELLOWORLD_SOURCES_CXX})
target_link_libraries(DDSPrintDataWriterQos fastrtps fastcdr tinyxml2::tinyxml2)

add_executable(DDSHelloWorldPublisherWithXml
    src/HelloWorldPublisherWithXml.cpp 
    src/ParticipantListener.cpp
    src/printDataWriterQos.cpp 
    ${DDS_HELLOWORLD_SOURCES_CXX})
target_link_libraries(DDSHelloWorldPublisherWithXml fastrtps fastcdr tinyxml2::tinyxml2)
add_dependencies(DDSHelloWorldPublisherWithXml copy_test_files)

add_executable(DDSHelloWorldSubscriberWithXml
    src/HelloWorldSubscriberWithXml.cpp 
    src/ParticipantListener.cpp
    src/printDataReaderQos.cpp 
    ${DDS_HELLOWORLD_SOURCES_CXX})
target_link_libraries(DDSHelloWorldSubscriberWithXml fastrtps fastcdr tinyxml2::tinyxml2)
add_dependencies(DDSHelloWorldSubscriberWithXml copy_test_files)

