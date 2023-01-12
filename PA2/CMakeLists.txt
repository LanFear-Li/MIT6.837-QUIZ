cmake_minimum_required(VERSION 3.16)
project(PA1)

set(CMAKE_CXX_STANDARD 11)

add_executable(raytracer
        main.C
        matrix.C
        image.C
        scene_parser.C
        camera.C
        object3d.C)