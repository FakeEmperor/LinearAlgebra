cmake_minimum_required(VERSION 3.6)
project(clion)

set(CMAKE_CXX_STANDARD 14)

include_directories(../src)
include_directories(../include)

file(GLOB SOURCES_ALGEBRA
        "../src/algebra/*.h"
        "../src/algebra/*.cc"
)
file(GLOB SOURCES_CODES
        "../src/codes/*.h"
        "../src/codes/*.cc"
)


add_executable(algebra ${SOURCES_ALGEBRA} ../src/codes/impl/BCHCode.h)
add_executable(codes ${SOURCES_CODES})
