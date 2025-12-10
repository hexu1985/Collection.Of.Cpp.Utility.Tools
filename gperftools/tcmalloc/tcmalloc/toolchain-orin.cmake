set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_SYSROOT "/opt/orin/gcc-9.3/aarch64-buildroot-linux-gnu/sysroot")
set(tools /opt/orin/gcc-9.3)
set(CMAKE_C_COMPILER ${tools}/bin/aarch64-buildroot-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER ${tools}/bin/aarch64-buildroot-linux-gnu-g++)
