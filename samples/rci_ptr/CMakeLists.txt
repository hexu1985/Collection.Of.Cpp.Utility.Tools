add_executable(benchmark_rci_ptr benchmark_rci_ptr.cpp widget.cpp)
target_link_libraries(benchmark_rci_ptr mini_utils-static ${LIBS_SYSTEM})

add_executable(sample_rci_ptr sample_rci_ptr.cpp widget.cpp)
target_link_libraries(sample_rci_ptr mini_utils-static ${LIBS_SYSTEM})

