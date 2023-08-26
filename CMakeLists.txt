cmake_minimum_required(VERSION 3.25.1)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ldl -lffi")
set(CMAKE_CXX_STANDARD 17)

project (banana)

file(GLOB SOURCES "lib/*.cpp")
add_library(banana_lib ${SOURCES})
target_link_libraries(banana_lib -lffi)

add_executable(banana src/banana.cpp)
target_link_libraries(banana -lffi banana_lib)

add_executable(assembler src/assembler.cpp)
target_link_libraries(assembler -lffi banana_lib)

add_executable(banana_test src/banana_test.cpp)
target_link_libraries(banana_test -lffi banana_lib)

# gtest
include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip)
FetchContent_MakeAvailable(googletest)
target_link_libraries(banana_test gtest_main)
add_test(BananaTest banana_test)
