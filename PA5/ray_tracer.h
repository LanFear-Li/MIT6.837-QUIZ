#ifndef _PA_RAY_TRACER_H_
#define _PA_RAY_TRACER_H_

#include <memory>

#include "input_parser.h"
#include "scene_parser.h"
#include "image.h"
#include "ray.h"
#include "hit.h"
#include "grid.h"

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

#endif
