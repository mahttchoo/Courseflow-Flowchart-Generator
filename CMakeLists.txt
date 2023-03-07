cmake_minimum_required(VERSION 3.22)
project(flowchart_the_fellowship)

set(CMAKE_CXX_STANDARD 14)

include_directories(./lemon-1.3.1)

include_directories(./lemon-1.3.1/build)

add_executable(flowchart_the_fellowship
        main.cpp coursenode.cpp coursenode.h)