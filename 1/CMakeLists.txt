cmake_minimum_required(VERSION 3.14)

project(RenamePack)

set(CMAKE_CXX_STANDARD 17)


file(GLOB_RECURSE SOURCE source/*.cpp)

add_executable(RenamePack ${SOURCE})

target_link_libraries(RenamePack
        -lpthread
        stdc++fs)