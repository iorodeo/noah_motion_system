cmake_minimum_required (VERSION 2.6)
project(captive_cos)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
link_directories("/usr/lib/arm-linux-gnueabihf/hdf5/serial/") # for rpi
add_executable(captive_cos main.cpp cos_trajectory.cpp)
target_link_libraries(captive_cos mctl atidaq usb armadillo hdf5_cpp hdf5)

