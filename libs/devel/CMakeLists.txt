cmake_minimum_required (VERSION 2.6)
project(devel)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_executable(
    devel
    main.cpp 
    rawhid_device.cpp 
    hid_LINUX.c
    )
target_link_libraries(devel usb)

