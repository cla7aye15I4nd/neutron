cmake_minimum_required(VERSION 3.14)
project(lck2)

set(CMAKE_CXX_STANDARD 14)

add_executable(lck2
#        main.cpp
        code.cpp
        BTree.hpp
        myVector.hpp
        bufferpool.hpp
        bpNode.hpp
        code.cpp)