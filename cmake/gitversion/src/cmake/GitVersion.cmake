# This module defines the following variables utilizing
# git to determine the parent tag. And if found the macro
# will attempt to parse them in the github tag format
#
# Useful for auto-versioning in our CMakeLists
#
#  ${VARPREFIX}_VERSION_MAJOR - Major version.
#  ${VARPREFIX}_VERSION_MINOR - Minor version
#  ${VARPREFIX}_VERSION_PATCH - Patch number
#  ${VARPREFIX}_VERSION_STAGE - Stage version
#  ${VARPREFIX}_COMMITID      - Git commitid
#  ${VARPREFIX}_BRANCH        - Git branch
#
# Example usage:
#
# fetch_version_from_git("test")
#    message("        major=${test_VERSION_MAJOR}")
#    message("        minor=${test_VERSION_MINOR}")
#    message("        patch=${test_VERSION_PATCH}")
#    message("        stage=${test_VERSION_STAGE}")
#    message("        stage=${test_BRANCH}")

include(FindGit)

macro(fetch_version_from_git VARPREFIX)
    # set our defaults.
    set(${VARPREFIX}_VERSION_MAJOR 1)
    set(${VARPREFIX}_VERSION_MINOR 0)
    set(${VARPREFIX}_VERSION_PATCH 0)
    set(${VARPREFIX}_VERSION_STAGE "alpha-dev")

    find_package(Git)

    if (GIT_FOUND)
        # 获取git的提交号
        execute_process(
            COMMAND
            ${GIT_EXECUTABLE} rev-parse  HEAD
            WORKING_DIRECTORY
            ${PROJECT_SOURCE_DIR}
            RESULT_VARIABLE
            GITRET
            OUTPUT_VARIABLE
            ${VARPREFIX}_COMMITID
            OUTPUT_STRIP_TRAILING_WHITESPACE
            )

        if (NOT GITRET EQUAL 0)
            message(FATAL_ERROR "Failed to fetch current git commitid.")
        endif()

        # 获取git的分支名
        execute_process(
            COMMAND
            ${GIT_EXECUTABLE} rev-parse  --abbrev-ref HEAD
            WORKING_DIRECTORY
            ${PROJECT_SOURCE_DIR}
            RESULT_VARIABLE
            GITRET
            OUTPUT_VARIABLE
            ${VARPREFIX}_BRANCH
            OUTPUT_STRIP_TRAILING_WHITESPACE
            )

        if (NOT GITRET EQUAL 0)
            message(FATAL_ERROR "Failed to fetch current git branch name.")
        endif()

        # 通过git的tag获取工程的版本号
        execute_process(
            COMMAND
            ${GIT_EXECUTABLE} describe --abbrev=0 --always
            WORKING_DIRECTORY
            ${PROJECT_SOURCE_DIR}
            RESULT_VARIABLE
            GITRET
            OUTPUT_VARIABLE
            GITVERSION
            OUTPUT_STRIP_TRAILING_WHITESPACE
            )

        # 匹配(.、_、-)，并替换为;  即将字符串分割成字符串列表
        # 获取到的版本信息如：release-2.2.1-alpha, 总共5段
        string(REGEX REPLACE "[\\._-]" ";" VERSION_LIST "${GITVERSION}")
        if(VERSION_LIST)
            list(LENGTH VERSION_LIST VERSION_LIST_LENGTH)
        endif()

        if ((GITRET EQUAL 0) AND (VERSION_LIST_LENGTH EQUAL 5))
            list(GET VERSION_LIST 1 _MAJOR)
            list(GET VERSION_LIST 2 _MINOR)
            list(GET VERSION_LIST 3 _PATCH)
            list(GET VERSION_LIST 4 _STAGE)

            set(_DEFAULT_VERSION "${${VARPREFIX}_GIT___VERSION_MAJOR}.${${VARPREFIX}_GIT___VERSION_MINOR}.${${VARPREFIX}_GIT___VERSION_PATCH}-${${VARPREFIX}_GIT___VERSION_STAGE}")
            set(_GIT_VERSION     "${_MAJOR}.${_MINOR}.${_PATCH}-${_STAGE}")

            if (${_DEFAULT_VERSION} VERSION_LESS ${_GIT_VERSION})
                set(${VARPREFIX}_VERSION_MAJOR ${_MAJOR})
                set(${VARPREFIX}_VERSION_MINOR ${_MINOR})
                set(${VARPREFIX}_VERSION_PATCH ${_PATCH})
                set(${VARPREFIX}_VERSION_STAGE ${_STAGE})
            endif()
        endif()
    endif()
endmacro()
