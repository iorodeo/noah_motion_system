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
    joystick/joystick.cpp
    trajectory.cpp
    )

link_directories("/usr/lib/arm-linux-gnueabihf/hdf5/serial/") # for rpi
link_directories("/usr/lib/x86_64-linux-gnu/hdf5/serial/")    # for newer

target_link_libraries(mctl usb armadillo hdf5_cpp hdf5)

# Install include files
file(GLOB mctl_INCLUDE_FILES ./ *.hpp  *.h)
file(GLOB mctl_INCLUDE_FILES ./ joystick/*.hpp)
file(GLOB mctl_filesystem_INCLUDE_FILES ./ filesystem/*.h)
file(GLOB mctl_json_INCLUDE_FILES ./ json/*.hpp)
file(GLOB mctl_firmware_INCLUDE_FILES ./ ../../../firmware/noah_controller/dev_constants.h)
file(GLOB mctl_firmware_INCLUDE_FILES ./ ../../../firmware/noah_controller/rawhid_msg_types.h)
install(FILES ${mctl_INCLUDE_FILES} DESTINATION include/mctl)
install(FILES ${mctl_filesystem_INCLUDE_FILES} DESTINATION include/mctl/filesystem)
install(FILES ${mctl_json_INCLUDE_FILES} DESTINATION include/mctl/json)
install(FILES ${mctl_firmware_INCLUDE_FILES} DESTINATION include/mctl)

# Install library
install(TARGETS mctl DESTINATION lib)
