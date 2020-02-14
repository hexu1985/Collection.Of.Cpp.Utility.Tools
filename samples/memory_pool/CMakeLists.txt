aux_source_directory(. SRC_FILE_LIST)
get_filename_component(EXE_FILE ${CMAKE_CURRENT_SOURCE_DIR} NAME_WE)
set(EXE_FILE ${EXE_FILE}_sample)
if (WIN32)
    set(EXE_FILE ${EXE_FILE}.exe)
endif()
add_executable(${EXE_FILE} ${SRC_FILE_LIST})
target_link_libraries(${EXE_FILE} mini_utils-static ${LIBS_SYSTEM})

