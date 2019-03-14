cmake_minimum_required(VERSION 3.13)
project(Table)

set(CMAKE_CXX_STANDARD 17)

add_executable(Table main.cpp src/htab.h src/htab.cpp)