project(devel_test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
    ${devel_test_SOURCE_DIR}/../../libs/mctl/
    ${devel_test_SOURCE_DIR}/../../../firmware/noah_controller/ 
    /usr/include/hdf5/serial/
    )

link_directories("/usr/lib/arm-linux-gnueabihf/hdf5/serial/") # for rpi
link_directories("/usr/lib/x86_64-linux-gnu/hdf5/serial/")    # for newer

add_executable(devel_test devel_test.cpp)
target_link_libraries(devel_test mctl atidaq)

