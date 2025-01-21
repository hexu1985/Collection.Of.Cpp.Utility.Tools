# This module defines the following variables to denote 
# new generated buildno
#
#  ${VARPREFIX}_BUILD_NUMBER  - New generated BuildNo
#
# If the build_number.txt file does not exist under 
# ${CMAKE_CURRENT_SOURCE_DIR} directory, a new build_number.txt 
# will be created and the build no will be set to 1, otherwise 
# the build no will increment one by one everytime cmake command 
# is executed.
#
# Example usage:
#
# new_build_number("test")
#    message("        buildno=${test_BUILD_NUMBER}")

macro(new_build_number VARPREFIX)
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/build_number.txt")
        file(READ "${CMAKE_CURRENT_SOURCE_DIR}/build_number.txt" ${VARPREFIX}_BUILD_NUMBER)
        string(STRIP ${${VARPREFIX}_BUILD_NUMBER} BUILD_NUMBER)
    else()
        set(${VARPREFIX}_BUILD_NUMBER  1)
    endif()

    # 将 build number 加1
    math(EXPR NEW_BUILD_NUMBER "${${VARPREFIX}_BUILD_NUMBER} + 1")

    # 写入新的 build number 到文件
    file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/build_number.txt" "${NEW_BUILD_NUMBER}")

endmacro()
