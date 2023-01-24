#include <iostream>

#include "ray_tracer.h"

const Vec3f BLACK(0, 0, 0);
const Vec3f WHITE(255, 255, 255);

RayTracer ray_tracer;

void render_magic() {
    // set image size and background
    InputParser input_parser = *ray_tracer.input_parser;
    Image render_image(input_parser.width, input_parser.height);
    Image depth_image(input_parser.width, input_parser.height);
    Image normal_image(input_parser.width, input_parser.height);

    // render image with ray caster
    ray_tracer.render_image(render_image, depth_image, normal_image,
                            input_parser.depth_min, input_parser.depth_max, input_parser.shade_back);

    // save ray caster render_image, depth image and normal image
    render_image.SaveTGA(input_parser.output_file);
    if (input_parser.depth_file != nullptr) {
        depth_image.SaveTGA(input_parser.depth_file);
    }

    if (input_parser.normal_file != nullptr) {
        normal_image.SaveTGA(input_parser.normal_file);
    }
}

void traceRay_magic(float x, float y) {

}

int main(int argc, char *argv[]) {
    // prepare input attributes and ray caster materials
    std::cout << "initializing..." << std::endl;
    InputParser input_parser(argc, argv);
    SceneParser scene_parser(input_parser.input_file);
    ray_tracer.init(&input_parser, &scene_parser);

    // render image with-or-without gui
    if (input_parser.gui) {
        glutInit(&argc, argv);
        GLCanvas gl_canvas;
        gl_canvas.initialize(ray_tracer.scene_parser, render_magic, traceRay_magic);
    } else {
        render_magic();
    }
    return 0;
}