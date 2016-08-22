cmake_minimum_required (VERSION 2.6)

# -----------------------------------------------------------------------------
#
# Notes:  use CXX=g++-4.9 cmake [path_to_source]/ to  use g++-4.9 compiler.
#
# -----------------------------------------------------------------------------

project(devel)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_executable(
    devel
    main.cpp 
    rawhid_device.cpp 
    motion_controller.cpp
    hid_LINUX.c
    )
target_link_libraries(devel usb)

