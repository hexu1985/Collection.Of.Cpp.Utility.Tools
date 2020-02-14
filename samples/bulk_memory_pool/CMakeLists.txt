aux_source_directory(. SRC_FILE_LIST)
set(EXE_FILE sample)
if (WIN32)
    set(EXE_FILE ${EXE_FILE}.exe)
endif()
add_executable(${EXE_FILE} ${SRC_FILE_LIST})
target_link_libraries(${EXE_FILE} mini_utils-static ${LIBS_SYSTEM})

