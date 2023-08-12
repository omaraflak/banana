cmake_minimum_required(VERSION 3.25.1)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

project (banana)

include_directories(lib)
file(GLOB SOURCES "lib/*.cpp")

add_executable(banana src/banana.cpp ${SOURCES})
add_executable(assembler src/assembler.cpp ${SOURCES})