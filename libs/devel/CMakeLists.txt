cmake_minimum_required (VERSION 2.6)

# -----------------------------------------------------------------------------
#
# Notes:  use CXX=g++-4.9 cmake [path_to_source]/ to  use g++-4.9 compiler.
#
# -----------------------------------------------------------------------------

project(devel)

# Add constants file from firmware
file(GLOB firmware_constants_SOURCE  ${devel_SOURCE_DIR}/../../firmware/noah_controller/constants.cpp)
include_directories(${devel_SOURCE_DIR}/../../firmware/noah_controller/)

# Add source files
file(GLOB devel_SOURCE RELATIVE ${devel_SOURCE_DIR} *.cpp *.c)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_executable(
    devel
    ${firmware_constants_SOURCE}
    ${devel_SOURCE}
    )
target_link_libraries(devel usb)

