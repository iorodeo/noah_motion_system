project(mcontrol)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
    ${mctl_SOURCE_DIR}/../../libs/docopt/
    ${mctl_SOURCE_DIR}/../../libs/mctl/
    ${mctl_SOURCE_DIR}/../../../firmware/noah_controller/ 
    /usr/include/hdf5/serial/
    )

add_executable(mcontrol cmd_line_app.cpp)

target_link_libraries(mcontrol mctl atidaq)

