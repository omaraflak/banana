cmake_minimum_required(VERSION 3.25.1)

set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -ldl -lffi")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ldl -lffi")
set(CMAKE_CXX_STANDARD 17)

project (banana)

file(GLOB SOURCES "src/lib/*.cpp")
add_library(banana_lib ${SOURCES})
target_link_libraries(banana_lib -lffi)

add_executable(banana src/banana.cpp)
target_link_libraries(banana -lffi banana_lib)

add_executable(assembler src/assembler.cpp)
target_link_libraries(assembler -lffi banana_lib)

add_subdirectory(lib/googletest)
add_executable(banana_test src/banana_test.cpp)
target_link_libraries(banana_test -lffi banana_lib gtest_main)
