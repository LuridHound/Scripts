cmake_minimum_required(VERSION 3.14)
project(CodeLine)

set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCE source/*.cpp)

add_executable(CodeLine ${SOURCE})

target_link_libraries(CodeLine
        stdc++fs
        -lpthread)
