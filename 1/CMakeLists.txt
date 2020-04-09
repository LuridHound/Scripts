cmake_minimum_required(VERSION 3.14)

project(RenamePack)

set(CMAKE_CXX_STANDARD 17)

add_executable(RenamePack main.cpp)

target_link_libraries(RenamePack
        stdc++fs)