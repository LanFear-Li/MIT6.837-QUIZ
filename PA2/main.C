#include <iostream>

#include "input_parser.h"
#include "ray_caster.h"

const Vec3f BLACK(0, 0, 0);
const Vec3f WHITE(255, 255, 255);

int main(int argc, char *argv[]) {
    std::cout << "initializing..." << std::endl;
    InputParser input_parser(argc, argv);

    // prepare ray caster materials
    RayCaster ray_caster{};
    ray_caster.get_scene(input_parser.input_file);

    // set image size and background
    Image render_image(input_parser.width, input_parser.height);
    Image depth_image(input_parser.width, input_parser.height);
    Image normal_image(input_parser.width, input_parser.height);

    // render image with ray caster
    ray_caster.render_image(render_image, depth_image, normal_image,
                            input_parser.depth_min, input_parser.depth_max, input_parser.shade_back);

    // save ray caster render_image, depth image and normal image
    std::cout << "Saving results to image..." << std::endl;
    render_image.SaveTGA(input_parser.output_file);

    if (input_parser.depth_file != nullptr) {
        depth_image.SaveTGA(input_parser.depth_file);
    }

    if (input_parser.normal_file != nullptr) {
        normal_image.SaveTGA(input_parser.normal_file);
    }
    return 0;
}