#include <iostream>
#include <cstring>
#include <cassert>

#include "scene_parser.h"
#include "ray.h"
#include "camera.h"
#include "hit.h"
#include "object3d.h"
#include "image.h"

int main(int argc, char *argv[]) {
    char *input_file = NULL;
    int width = 100;
    int height = 100;
    char *output_file = NULL;
    float depth_min = 0;
    float depth_max = 1;
    char *depth_file = NULL;

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
        } else {
            printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
            assert(0);
        }
    }


    SceneParser scene_parser(input_file);
    Camera *camera = scene_parser.getCamera();
    Group *group = scene_parser.getGroup();

    Image image(width, height);
    image.SetAllPixels()

    // bottom_left -> (0, 0), upper_right -> (width, height)
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            float u = i / width;
            float v = j / height;

            Hit hit;
            Ray ray = camera->generateRay(Vec2f(u, v));
            group->intersect(ray, hit, camera->getTMin());

            
        }
    }
}