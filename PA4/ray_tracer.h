#ifndef _RAY_CASTER_H
#define _RAY_CASTER_H

#include <memory>

#include "input_parser.h"
#include "scene_parser.h"
#include "glCanvas.h"
#include "image.h"
#include "camera.h"
#include "light.h"

extern const Vec3f BLACK;
extern const Vec3f WHITE;

class RayTracer {
public:
    void init(InputParser *input, SceneParser *scene);

    void render_image(Image &output_image, Image &depth_image, Image &normal_image,
                      float depth_min, float depth_max, bool shade_back) const;

    Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight, float indexOfRefraction, Hit &hit) const;

    ~RayTracer();

    InputParser *input_parser;

    SceneParser *scene_parser;
};

#endif //_RAY_CASTER_H
