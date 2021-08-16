aux_source_directory(. SRC_FILE_LIST)
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME_WE)
set(EXE_FILE sample_${DIR_NAME})
if (WIN32)
    set(EXE_FILE ${EXE_FILE}.exe)
endif()
add_executable(${EXE_FILE} ${SRC_FILE_LIST})
target_link_libraries(${EXE_FILE} mini_util-static ${LIBS_SYSTEM})

