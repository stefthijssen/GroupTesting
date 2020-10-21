project(GroupTesting)

cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -g")
add_executable(GroupTesting groupTesting.cpp)
add_executable(GroupTestingAdjacentyMatrix groupTestingAdjacentyMatrix.cpp)
