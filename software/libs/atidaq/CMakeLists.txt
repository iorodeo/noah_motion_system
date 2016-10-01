project(atidaq)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(atidaq_SRC ftconfig.c ftrt.c dom.c expatls.c node.c stack.c xmlparse.c xmlrole.c xmltok.c util.c sixax.c)

add_library(atidaq ${atidaq_SRC})
install(TARGETS atidaq DESTINATION lib)


