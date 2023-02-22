cmake_minimum_required(VERSION 3.16)
project(PA8)

set(CMAKE_CXX_STANDARD 11)

add_executable(curve_editor
        main.cpp

        arg_parser.h
        glCanvas.cpp
        spline_parser.cpp

        triangle_mesh.cpp
        matrix.cpp
        vectors.h

        curve.cpp
        spline.cpp
        surface.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

target_link_libraries(curve_editor -lGL -lglut -lGLU)