aux_source_directory(. SRC_FILE_LIST)

get_filename_component(CURRENT_DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME_WE)
if (DIR_NAME)
    set(DIR_NAME ${DIR_NAME}_${CURRENT_DIR_NAME})
else()
    set(DIR_NAME ${CURRENT_DIR_NAME})
endif()

set(EXE_FILE sample_${DIR_NAME})
if (WIN32)
    set(EXE_FILE ${EXE_FILE}.exe)
endif()
add_executable(${EXE_FILE} ${SRC_FILE_LIST})
target_link_libraries(${EXE_FILE} mini_utils-static ${LIBS_SYSTEM})

