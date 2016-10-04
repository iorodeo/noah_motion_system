cmake_minimum_required (VERSION 2.6)
project(basic)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
link_directories("/usr/lib/arm-linux-gnueabihf/hdf5/serial/") # for rpi
add_executable(basic basic.cpp)
target_link_libraries(basic mctl atidaq usb armadillo hdf5_cpp hdf5)

