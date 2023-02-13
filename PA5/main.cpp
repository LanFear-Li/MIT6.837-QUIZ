#include <iostream>

#include "ray_tracer.h"
#include "glCanvas.h"

RayTracer ray_tracer;

void render_magic() {
    cout << "enter render_magic..." << endl;
    // set image size and background
    InputParser input_parser = *ray_tracer.input_parser;
    Image render_image(input_parser.width, input_parser.height);
    Image depth_image(input_parser.width, input_parser.height);
    Image normal_image(input_parser.width, input_parser.height);

    // render image with ray caster
    ray_tracer.render(render_image, depth_image, normal_image);

    // save ray caster render, depth image and normal image
    render_image.SaveTGA(input_parser.output_file);
    if (input_parser.depth_file != nullptr) {
        depth_image.SaveTGA(input_parser.depth_file);
    }

    if (input_parser.normal_file != nullptr) {
        normal_image.SaveTGA(input_parser.normal_file);
    }
}

void raytrace_magic(float x, float y) {
    cout << "enter raytrace_magic..." << endl;
    auto *camera = ray_tracer.scene_parser->getCamera();
    assert(camera != nullptr);
    Vec2f p(x, y);
    auto ray = camera->generateRay(p);
    Hit hit;
    ray_tracer.traceRay(ray, camera->getTMin(), 0, 1.0f, 1.0f, hit);
}

int main(int argc, char *argv[]) {
    // prepare input attributes and ray caster materials
    cout << "raytracer initializing..." << std::endl;
    InputParser input_parser(argc, argv);
    SceneParser scene_parser(input_parser.input_file);
    ray_tracer.init(&input_parser, &scene_parser);

    // render image with-or-without gui
    if (input_parser.gui) {
        glutInit(&argc, argv);
        GLCanvas gl_canvas;
        gl_canvas.initialize(ray_tracer.scene_parser, render_magic, raytrace_magic, ray_tracer.grid_ptr, input_parser.visualize_grid);
    } else {
        render_magic();
    }
    return 0;
}