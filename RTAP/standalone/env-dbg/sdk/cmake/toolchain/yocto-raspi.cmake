#

message(" -- CMake Linux for Yocto Raspi toolchain -- ")
# Standard settings
set (CMAKE_SYSTEM_NAME Linux)
set (CMAKE_SYSTEM_VERSION 1)
set (UNIX True)
set (JVX_CROSS_COMPILE True)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

#link_directories(SYSTEM "${CMAKE_FIND_ROOT_PATH}/usr/include")
#include_directories(SYSTEM /usr/local/arm/4.3.1-eabi-armv6/usr/include)
