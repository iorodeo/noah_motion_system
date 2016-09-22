project(mctl)

file(GLOB firmware_constants_SOURCE  ${mctl_SOURCE_DIR}/../../../firmware/noah_controller/dev_constants.cpp)

include_directories(
    ${mctl_SOURCE_DIR}/../../../firmware/noah_controller/ 
    ${mctl_SOURCE_DIR}/../atidaq/
    /usr/include/hdf5/serial/
    )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_library(
    mctl
    ${firmware_constants_SOURCE}
    hid_LINUX.c
    rtn_status.cpp
    rawhid_device.cpp
    constants.cpp
    controller.cpp
    configuration.cpp
    outscan_data.cpp
    ft_sensor_cal.cpp
    ft_tool_transform.cpp
    )

link_directories("/usr/lib/arm-linux-gnueabihf/hdf5/serial/") # for rpi

target_link_libraries(mctl usb armadillo hdf5_cpp hdf5 docopt)

