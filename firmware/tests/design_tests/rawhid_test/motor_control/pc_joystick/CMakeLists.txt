cmake_minimum_required (VERSION 2.6)
project(motor_control_test)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_executable(motor_control_test main.cpp dynamic_model.cpp joystick.cpp rawhid_device.cpp hid_LINUX.c)
target_link_libraries( motor_control_test usb)

