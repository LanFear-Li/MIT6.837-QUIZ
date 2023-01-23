cmake_minimum_required(VERSION 3.16)
project(PA4)

set(CMAKE_CXX_STANDARD 11)

add_executable(raytracer
        main.C
        matrix.C
        image.C
        material.C
        light.C
        camera.C
        object3d.C
        glCanvas.C
        scene_parser.C
        input_parser.C
        ray_tracer.C
        rayTree.C
        )

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

if (OPENGL_FOUND)
    message("opengl found")
    message("include dir: ${OPENGL_INCLUDE_DIR}")
    message("link libraries: ${OPENGL_gl_LIBRARY}")
else (OPENGL_FOUND)
    message("opengl not found")
endif()

target_link_libraries(raytracer -lGL -lglut -lGLU)