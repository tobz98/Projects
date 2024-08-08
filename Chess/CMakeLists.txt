cmake_minimum_required(VERSION 3.26)
project(ProjectRedo)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})

add_executable(ProjectRedo main.cpp
        Bitboard.cpp
        Bitboard.h
        Definitions.h
        Board.cpp
        Board.h
        AttackMasks.cpp
        AttackMasks.h
        Move.cpp
        Move.h
        MoveList.h
        Move.cpp
        Evaluation.cpp
        Evaluation.h
)

target_link_libraries(ProjectRedo ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})