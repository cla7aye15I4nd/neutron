cmake_minimum_required(VERSION 3.14)
project(bptree)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(bptree
        bptree.hpp
        main.cpp
#        code.cpp
        )
