cmake_minimum_required(VERSION 3.31)
project(haptic_library)

set(CMAKE_CXX_STANDARD 26)
find_package(PkgConfig REQUIRED)

add_library(haptic_library SHARED library.cpp)

add_executable(haptic_test test.cpp)
