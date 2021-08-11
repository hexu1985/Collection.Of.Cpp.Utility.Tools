add_executable(sample_basic_stopwatch1 sample_basic_stopwatch1.cpp)
target_link_libraries(sample_basic_stopwatch1 mini_utils-static ${LIBS_SYSTEM})

if (NOT WIN32)
    add_executable(sample_basic_stopwatch2 sample_basic_stopwatch2.cpp)
    target_link_libraries(sample_basic_stopwatch2 mini_utils-static ${LIBS_SYSTEM})
endif()

add_executable(sample_stopwatch1 sample_stopwatch1.cpp)
target_link_libraries(sample_stopwatch1 mini_utils-static ${LIBS_SYSTEM})

add_executable(sample_timer_base_chrono1 sample_timer_base_chrono1.cpp)
target_link_libraries(sample_timer_base_chrono1 mini_utils-static ${LIBS_SYSTEM})
