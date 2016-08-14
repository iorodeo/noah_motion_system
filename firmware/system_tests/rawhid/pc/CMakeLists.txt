cmake_minimum_required (VERSION 2.6)
project(rawhid_test)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_executable(
    rawhid_test
    main.cpp 
    rawhid_device.cpp 
    hid_LINUX.c
    )
target_link_libraries(rawhid_test usb)

