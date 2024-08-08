cmake_minimum_required(VERSION 3.26)
project(Noughts_and_Crosses)

set(CMAKE_CXX_STANDARD 17)

add_executable(Noughts_and_Crosses main.cpp
        Game.cpp
        Game.h)
