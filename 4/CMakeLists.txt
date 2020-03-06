cmake_minimum_required(VERSION 3.14)
#TODO. Rename gamfn(get and modificate filenames).
project(gamfn)

set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 17)

add_executable(crilf main.cpp)

target_link_libraries(${PROJECT_NAME}
        stdc++fs)
