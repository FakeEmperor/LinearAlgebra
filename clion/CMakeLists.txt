cmake_minimum_required(VERSION 3.6)
project(clion)

set(CMAKE_CXX_STANDARD 14)

include_directories(../src)
include_directories(../include)

file(GLOB SOURCES_UTILS
        "../src/utils/*.h"
        "../src/utils/*.cc"
)

file(GLOB SOURCES_ALGEBRA
        "../src/algebra/*.h"
        "../src/algebra/finite/*.h"
        "../src/algebra/finite/impl*.h"

        "../src/algebra/*.cc"
        "../src/algebra/finite/*.cc"
        "../src/algebra/finite/impl/*.cc"

        "../src/algebra/utils/*.h"
        "../src/algebra/utils/*.cc"
)
file(GLOB SOURCES_CODES
        "../src/codes/*.h"
        "../src/codes/*.cc"
)

file(GLOB SOURCES_TEST
        "../tests/catch.hpp"
)

if(CMAKE_BUILD_TYPE MATCHES Test)
    file(GLOB MAIN_ALGEBRA ${SOURCES_TEST} "../tests/test_algebra.cc")
    file(GLOB MAIN_CODES ${SOURCES_TEST} "../tests/test_codes.cc")
else()
    file(GLOB MAIN_ALGEBRA "../src/examples/examples_algebra.cc")
    file(GLOB MAIN_CODES "../src/examples/examples_codes.cc")
endif(CMAKE_BUILD_TYPE MATCHES Test)

add_executable(algebra ${SOURCES_UTILS} ${SOURCES_ALGEBRA}
        ${MAIN_ALGEBRA})
add_executable(codes ${SOURCES_UTILS} ${SOURCES_ALGEBRA} ${SOURCES_CODES}
        ${MAIN_CODES})


