cmake_minimum_required(VERSION 3.17)
project(mul)

set(CMAKE_CXX_STANDARD 20)

add_executable(test_mul test_mul.cpp)
add_executable(handmake ./handmake/handmake.cpp)