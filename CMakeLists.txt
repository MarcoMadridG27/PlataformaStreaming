cmake_minimum_required(VERSION 3.27)
project(pruebas)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(MovieTreeTest
        src/main.cpp
        src/MovieTree.cpp
        src/Utils.cpp
        src/Movie.cpp
)