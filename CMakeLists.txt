cmake_minimum_required(VERSION 3.13)
project(neutron)

set(CMAKE_CXX_STANDARD 14)

add_executable(neutron
        neutron/main.cpp
        )
