#include "ray_tracer.h"

float f_clamp(float v, float min = 0, float max = 1) {
    float ans = v;
    if (ans < min) ans = min;
    if (ans > max) ans = max;
    return ans;
}

void RayTracer::init(InputParser *input, SceneParser *scene) {
    this->input_parser = input;
    this->scene_parser = scene;
}

RayTracer::~RayTracer() {
    delete this->input_parser;
    delete this->scene_parser;
}

void RayTracer::render_image(Image &output_image, Image &depth_image, Image &normal_image,
                             float depth_min, float depth_max, bool shade_back) const {
    output_image.SetAllPixels(scene_parser->getBackgroundColor());
    normal_image.SetAllPixels(BLACK);

    Camera *camera = scene_parser->getCamera();
    Group *group = scene_parser->getGroup();
    Vec3f ambient_color = scene_parser->getAmbientLight();

    int width = output_image.Width(), height = output_image.Height();

    // bottom_left -> (0, 0), upper_right -> (width, height)
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
                        output_image.SetPixel(i, j, BLACK);
                        continue;
                    }
                }

                // diffuse shading
                Vec3f object_color = hit.getMaterial()->getDiffuseColor();
                Vec3f render_color = ambient_color * object_color;

                int light_num = scene_parser->getNumLights();
                for (int k = 0; k < light_num; k++) {
                    Vec3f dir_to_light, light_color;
                    float dis_to_light;

                    scene_parser->getLight(k)->getIllumination(Vec3f(), dir_to_light, light_color, dis_to_light);
                    render_color += material_ptr->Shade(ray, hit, dir_to_light, light_color);
                }
                output_image.SetPixel(i, j, render_color);

                // depth_image visualization
                t = f_clamp(t, depth_min, depth_max);
                t = 1.0f - (t - depth_min) / (depth_max - depth_min);
                Vec3f depth_color(t, t, t);
                depth_image.SetPixel(i, j, depth_color);

                // normal_image visualization
                // x, y and z [-1, 1] -> [0, 1]
                float x = fabs(normal.x());
                float y = fabs(normal.y());
                float z = fabs(normal.z());
                Vec3f normal_color(x, y, z);
                normal_image.SetPixel(i, j, normal_color);
            }
        }
    }
}

Vec3f RayTracer::traceRay(Ray &ray, float tmin, int bounces, float weight, float indexOfRefraction, Hit &hit) const {
    return Vec3f();
}



