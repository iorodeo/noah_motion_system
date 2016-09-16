# -----------------------------------------------------------------------------
# Notes:  use CC=gcc-4.6 CXX=g++-4.9 cmake [path_to_source]/ to  use g++-4.9 compiler.
# -----------------------------------------------------------------------------
cmake_minimum_required (VERSION 2.6)

add_subdirectory("libs/docopt")
add_subdirectory("libs/atidaq")
add_subdirectory("libs/motion")
add_subdirectory("apps/cmd_line")
add_subdirectory("apps/devel")
