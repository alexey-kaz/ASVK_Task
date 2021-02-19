cmake_minimum_required(VERSION 3.15)
project(ASVK)

set(CMAKE_CXX_STANDARD 14)

add_executable(ASVK main.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")
