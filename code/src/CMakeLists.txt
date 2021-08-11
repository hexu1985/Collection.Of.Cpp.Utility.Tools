file(GLOB MINI_UTILS_LIB_SRC_LIST *.cpp)

add_library(mini_utils SHARED ${MINI_UTILS_LIB_SRC_LIST})
add_library(mini_utils-static STATIC ${MINI_UTILS_LIB_SRC_LIST})


