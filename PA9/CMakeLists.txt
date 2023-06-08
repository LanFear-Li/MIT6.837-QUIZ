cmake_minimum_required(VERSION 3.16)
project(PA9)

set(CMAKE_CXX_STANDARD 11)

add_executable(particle_system
        main.cpp

        matrix.cpp
        glCanvas.cpp
        system.cpp
        particle.cpp
        parser.cpp

        forcefield.cpp
        integrator.cpp
        generator.cpp

        random.h)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

target_link_libraries(particle_system -lGL -lglut -lGLU)