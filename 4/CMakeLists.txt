cmake_minimum_required(VERSION 3.14)

project(gamfn)

set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 17)

add_executable(gamfn main.cpp)

target_link_libraries(gamfn
        stdc++fs)
