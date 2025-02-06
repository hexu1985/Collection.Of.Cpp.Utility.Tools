cmake_minimum_required (VERSION 3.13)

set (CMAKE_CXX_FLAGS "-g -std=c++0x")

find_package (vsomeip3 3.4.10 REQUIRED)
find_package( Boost COMPONENTS system thread log REQUIRED )

include_directories (
    ${Boost_INCLUDE_DIR}
    ${VSOMEIP_INCLUDE_DIRS}
)

add_executable(service-example service-example.cpp)
target_link_libraries(service-example vsomeip3 ${Boost_LIBRARIES})
