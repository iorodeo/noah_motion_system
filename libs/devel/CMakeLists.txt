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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_library(
    motion
    ${firmware_constants_SOURCE}
    hid_LINUX.c
    docopt.cpp
    rtn_status.cpp
    rawhid_device.cpp
    motion_constants.cpp
    motion_controller.cpp
    motion_config.cpp
    )
target_link_libraries(motion usb armadillo)

add_executable(outscan_test main.cpp)
target_link_libraries(outscan_test motion)

add_executable(mctrl cmd_line_app.cpp)
target_link_libraries(mctrl motion)


