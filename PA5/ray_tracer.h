#ifndef _RAY_CASTER_H
#define _RAY_CASTER_H

#include <memory>

#include "input_parser.h"
#include "scene_parser.h"
#include "glCanvas.h"
#include "image.h"
#include "camera.h"
#include "light.h"
#include "rayTree.h"

extern const Vec3f BLACK;
extern const Vec3f WHITE;

class RayTracer {
public:
    void init(InputParser *input, SceneParser *scene);

    void render(Image &output_image, Image &depth_image, Image &normal_image) const;

    static Vec3f mirrorDirection(const Vec3f &normal, const Vec3f &incoming);

    static bool transmittedDirection(const Vec3f &normal, const Vec3f &incoming, float index_i, float index_t,
                              Vec3f &transmitted) ;

    Vec3f traceRay(Ray &ray, float t_min, int bounces, float weight, float indexOfRefraction, Hit &hit) const;

    ~RayTracer();

    InputParser *input_parser;

    SceneParser *scene_parser;

    Grid *grid_ptr;
};

#endif //_RAY_CASTER_H
