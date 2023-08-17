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
add_executable(banana_test src/banana_test.cpp ${SOURCES})

# gtest
include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip)
FetchContent_MakeAvailable(googletest)
target_link_libraries(banana_test gtest_main)
add_test(BananaTest banana_test)
