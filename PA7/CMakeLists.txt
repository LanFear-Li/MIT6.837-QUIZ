cmake_minimum_required(VERSION 3.16)
project(PA7)

set(CMAKE_CXX_STANDARD 11)

add_executable(raytracer
        main.cpp

        matrix.cpp
        image.cpp
        light.cpp
        camera.cpp
        glCanvas.cpp
        boundingbox.cpp
        rayTree.cpp
        material.cpp

        object3d.cpp
        triangle.cpp
        sphere.cpp
        plane.cpp
        grid.cpp
        group.cpp
        transform.cpp

        scene_parser.cpp
        input_parser.cpp
        ray_tracer.cpp
        raytracing_stats.cpp

        perlin_noise.cpp

        color.h

        sample.h
        film.cpp
        sampler.cpp
        filter.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

target_link_libraries(raytracer -lGL -lglut -lGLU)