# -----------------------------------------------------------------------------
# Notes:  use CC=gcc-4.9 CXX=g++-4.9 cmake [path_to_source]/ to  use g++-4.9 compiler.
# -----------------------------------------------------------------------------
cmake_minimum_required (VERSION 2.6)

add_subdirectory("libs/docopt")
add_subdirectory("libs/atidaq")
add_subdirectory("libs/mctl")
add_subdirectory("apps/mcontrol")
add_subdirectory("apps/devel")

message(STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
