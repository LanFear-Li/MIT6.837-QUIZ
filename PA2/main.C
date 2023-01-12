#include <iostream>
#include <cstring>
#include <cassert>
#include <algorithm>

#include "scene_parser.h"
#include "ray.h"
#include "camera.h"
#include "hit.h"
#include "object3d.h"
#include "image.h"
#include "material.h"

float f_clamp(float v, float min, float max) {
    float ans = v;
    if (ans < min) ans = min;
    if (ans > max) ans = max;
    return ans;
}

int main(int argc, char *argv[]) {
    char *input_file = nullptr;
    int width = 100;
    int height = 100;
    char *output_file = nullptr;
    float depth_min = 0;
    float depth_max = 1;
    char *depth_file = nullptr;
    char *normal_file = nullptr;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-input")) {
            i++;
            assert (i < argc);
            input_file = argv[i];
        } else if (!strcmp(argv[i], "-size")) {
            i++;
            assert (i < argc);
            width = atoi(argv[i]);
            i++;
            assert (i < argc);
            height = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-output")) {
            i++;
            assert (i < argc);
            output_file = argv[i];
        } else if (!strcmp(argv[i], "-depth")) {
            i++;
            assert (i < argc);
            depth_min = atof(argv[i]);
            i++;
            assert (i < argc);
            depth_max = atof(argv[i]);
            i++;
            assert (i < argc);
            depth_file = argv[i];
        } else if (!strcmp(argv[i], "-normal")) {
            i++;
            assert (i < argc);
            normal_file = argv[i];
        } else {
            printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
            assert(0);
        }
    }

    // prepare ray caster materials
    SceneParser scene_parser(input_file);
    Camera *camera = scene_parser.getCamera();
    Group *group = scene_parser.getGroup();

    // set render_image size and background
    Image render_image(width, height);
    Image depth_image(width, height);
    render_image.SetAllPixels(scene_parser.getBackgroundColor());
    // bottom_left -> (0, 0), upper_right -> (width, height)
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            float u = i * 1.0f / width;
            float v = j * 1.0f / height;

            Hit hit;
            Ray ray = camera->generateRay(Vec2f(u, v));
            group->intersect(ray, hit, camera->getTMin());

            if (hit.getMaterial() != nullptr) {
                Vec3f render_color = hit.getMaterial()->getDiffuseColor();
                render_image.SetPixel(i, j, render_color);

                // cast depth value t into gray color with clamp
                float t = hit.getT();
                t = f_clamp(t, depth_min, depth_max);
                t = 1.0f - (t - depth_min) / (depth_max - depth_min);
                Vec3f depth_color(t, t, t);
                depth_image.SetPixel(i, j, depth_color);
            }
        }
    }

    // save ray caster render_image and depth_image
    render_image.SaveTGA(output_file);
    depth_image.SaveTGA(depth_file);
    return 0;
}