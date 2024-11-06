cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(CPMFmtExample)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

#CPMAddPackage("gh:fmtlib/fmt#7.1.3")
CPMAddPackage(
    NAME fmt
    URL "https://mirrors.phigent.io/nfs/pkgs/fmt_7.0.3/x86_64-ubuntu-linux-gcc9.3.0/fmt_7.0.3.tar")

# ---- Executable ----

add_executable(CPMFmtExample main.cpp)
target_compile_features(CPMFmtExample PRIVATE cxx_std_17)
target_link_libraries(CPMFmtExample fmt)
