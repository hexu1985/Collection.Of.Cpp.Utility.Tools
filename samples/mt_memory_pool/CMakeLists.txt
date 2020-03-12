get_filename_component(CURRENT_DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME_WE)

if (DIR_NAME)
    set(DIR_NAME ${DIR_NAME}_${CURRENT_DIR_NAME})
else()
    set(DIR_NAME ${CURRENT_DIR_NAME})
endif()

add_subdirectory(dummylock)
add_subdirectory(mutex)
add_subdirectory(spinlock)
