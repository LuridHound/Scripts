cmake_minimum_required(VERSION 3.14)
project(CodeLine)

set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 17)

add_executable(CodeLine main.cpp)

target_link_libraries(${PROJECT_NAME}
        stdc++fs)
