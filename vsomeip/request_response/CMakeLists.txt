cmake_minimum_required (VERSION 3.13)

set (CMAKE_CXX_FLAGS "-g -std=c++0x")

find_package (vsomeip3 3.4.10 REQUIRED)
find_package( Boost COMPONENTS system thread log REQUIRED )

include_directories (
    ${Boost_INCLUDE_DIR}
    ${VSOMEIP_INCLUDE_DIRS}
)

add_executable(response-sample.cpp)
target_link_libraries(response-samplevsomeip3 ${Boost_LIBRARIES})

add_executable(hello_world_client hello_world_client_main.cpp)
target_link_libraries(hello_world_client vsomeip3 ${Boost_LIBRARIES})
