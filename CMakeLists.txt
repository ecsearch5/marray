cmake_minimum_required(VERSION 3.16)
project(marray)

set(CMAKE_CXX_STANDARD 14)

find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
enable_testing()

add_executable(test_library test_library.cpp)
target_link_libraries(test_library ${GTEST_BOTH_LIBRARIES} pthread)


