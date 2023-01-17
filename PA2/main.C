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
#include "light.h"

const Vec3f BLACK(0, 0, 0);
const Vec3f WHITE(255, 255, 255);

char *input_file = nullptr;
int width = 100;
int height = 100;

float depth_min = 0;
float depth_max = 1;
char *output_file = nullptr;
char *depth_file = nullptr;
char *normal_file = nullptr;

bool shade_back = false;

float f_clamp(float v, float min = 0, float max = 1) {
    float ans = v;
    if (ans < min) ans = min;
    if (ans > max) ans = max;
    return ans;
}

void input_parser(int argc, char *argv[]) {
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
        } else if (!strcmp(argv[i], "-normals")) {
            i++;
            assert (i < argc);
            normal_file = argv[i];
        } else if (!strcmp(argv[i], "-shade_back")) {
            shade_back = true;
        } else {
            printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
            assert(0);
        }
    }
}

int main(int argc, char *argv[]) {
    std::cout << "initializing..." << std::endl;
    input_parser(argc, argv);

    // prepare ray caster materials
    SceneParser scene_parser(input_file);
    Camera *camera = scene_parser.getCamera();
    Group *group = scene_parser.getGroup();

    // set render_image size and background
    Image render_image(width, height);
    Image depth_image(width, height);
    Image normal_image(width, height);
    render_image.SetAllPixels(scene_parser.getBackgroundColor());
    normal_image.SetAllPixels(BLACK);

    Vec3f ambient_color = Vec3f(0.1, 0.1, 0.1);

    // bottom_left -> (0, 0), upper_right -> (width, height)
    std::cout << "rendering..." << std::endl;
    std::cout << "shade back: " << shade_back << std::endl;
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            float u = i * 1.0f / width;
            float v = j * 1.0f / height;

            Hit hit;
            Ray ray = camera->generateRay(Vec2f(u, v));
            group->intersect(ray, hit, camera->getTMin());

            Material *material_ptr = hit.getMaterial();
            if (material_ptr != nullptr) {
                float t = hit.getT();
                Vec3f normal = hit.getNormal();

                // enable back face shading
                if (normal.Dot3(ray.getDirection()) > 0) {
                    if (shade_back) {
                        normal *= -1;
                    } else {
                        render_image.SetPixel(i, j, BLACK);
                        continue;
                    }
                }

                // diffuse shading
                Vec3f object_color = hit.getMaterial()->getDiffuseColor();
                Vec3f render_color = ambient_color * object_color;

                int light_num = scene_parser.getNumLights();
                for (int k = 0; k < light_num; k++) {
                    Vec3f dir, light_color;

                    scene_parser.getLight(k)->getIllumination(Vec3f(), dir, light_color);
                    Vec3f diffuse_color = f_clamp(dir.Dot3(normal)) * light_color * object_color;

                    render_color += diffuse_color;
                }
                render_image.SetPixel(i, j, render_color);

                // depth visualization
                t = f_clamp(t, depth_min, depth_max);
                t = 1.0f - (t - depth_min) / (depth_max - depth_min);
                Vec3f depth_color(t, t, t);
                depth_image.SetPixel(i, j, depth_color);

                // normal visualization
                // x and y [-1, 1] -> [0, 255 / 255]
                float x = (normal.x() + 1) / 2;
                float y = (normal.y() + 1) / 2;
                // z [0, 1] -> [128 / 255, 255 / 255]
                float z = normal.z() * (127.0f / 255) + 128.0f / 255;
                Vec3f normal_color(x, y, z);
                normal_image.SetPixel(i, j, normal_color);
            }
        }
    }

    std::cout << "Saving results to image..." << std::endl;
    // save ray caster render_image, depth image and normal image
    render_image.SaveTGA(output_file);

    if (depth_file != nullptr) {
        depth_image.SaveTGA(depth_file);
    }

    if (normal_file != nullptr) {
        normal_image.SaveTGA(normal_file);
    }
    return 0;
}