cmake_minimum_required (VERSION 2.6)
project(client)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories("~/src/socket.io-client-cpp/socket.io-client-cpp/build/include")
link_directories("~/src/socket.io-client-cpp/socket.io-client-cpp/build/lib/Release")

add_executable(client main.cpp)
target_link_libraries(client sioclient boost_system boost_date_time boost_random pthread)
