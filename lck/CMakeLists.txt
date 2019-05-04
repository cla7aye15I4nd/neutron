cmake_minimum_required(VERSION 3.14)
project(lck)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(lck
        bptree.hpp
        BTree.hpp
        exceptions.hpp
        main.cpp
        myVector.hpp
        utility.hpp
        vector.hpp)
