project(devel)

file(GLOB firmware_constants_SOURCE  ${devel_SOURCE_DIR}/../../../firmware/noah_controller/dev_constants.cpp)

include_directories(${devel_SOURCE_DIR}/../../../firmware/noah_controller/ /usr/include/hdf5/serial/)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_library(
    motion
    ${firmware_constants_SOURCE}
    hid_LINUX.c
    rtn_status.cpp
    rawhid_device.cpp
    constants.cpp
    controller.cpp
    configuration.cpp
    outscan_data.cpp
    )

link_directories("/usr/lib/arm-linux-gnueabihf/hdf5/serial/") # for rpi

target_link_libraries(motion usb armadillo hdf5_cpp hdf5 docopt)

