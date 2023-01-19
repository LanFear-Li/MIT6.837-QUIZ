#ifndef _RAY_CASTER_H
#define _RAY_CASTER_H

#include <memory>

#include "glCanvas.h"
#include "scene_parser.h"
#include "image.h"
#include "camera.h"
#include "light.h"

extern const Vec3f BLACK;
extern const Vec3f WHITE;

class RayCaster {
public:
    void get_scene(char *input_file);

    void render_image(Image &output_image, Image &depth_image, Image &normal_image,
                      float depth_min, float depth_max, bool shade_back) const;

    ~RayCaster();

    SceneParser *scene_parser;
};

#endif //_RAY_CASTER_H
