set(CMAKE_C_COMPILER /usr/bin/gcc)
cmake_minimum_required(VERSION 3.0)
project(project3)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES main.cpp Kernel.cpp Kernel_util.cpp Memory.cpp Process.cpp Process_util.cpp Pmemory.cpp)

add_executable(project3 ${SOURCES})