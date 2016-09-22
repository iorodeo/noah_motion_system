project(devel_test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
    ${devel_test_SOURCE_DIR}/../../libs/motion/
    ${devel_test_SOURCE_DIR}/../../../firmware/noah_controller/ 
    /usr/include/hdf5/serial/
    )

add_executable(devel_test devel_test.cpp)

target_link_libraries(devel_test motion atidaq)

