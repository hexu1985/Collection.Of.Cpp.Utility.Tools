add_executable(benchmark_rc_ptr benchmark_rc_ptr.cpp widget.cpp)
target_link_libraries(benchmark_rc_ptr mini_utils-static ${LIBS_SYSTEM})

add_executable(sample_rc_ptr sample_rc_ptr.cpp widget.cpp)
target_link_libraries(sample_rc_ptr mini_utils-static ${LIBS_SYSTEM})

