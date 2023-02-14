#include "ray_tracer.h"

#include "rayTree.h"
#include "color.h"

float f_clamp(float v, float min = 0, float max = 1) {
    float ans = v;
    if (ans < min) ans = min;
    if (ans > max) ans = max;
    return ans;
}

void RayTracer::init(InputParser *input, SceneParser *scene) {
    input_parser = input;
    scene_parser = scene;

    if (input_parser->with_grid) {
        Group *group_ptr = scene_parser->getGroup();
        grid_ptr = new Grid(group_ptr->getBoundingBox(), input_parser->nx, input_parser->ny, input_parser->nz);
        group_ptr->insertIntoGrid(grid_ptr, nullptr);
    }
}

RayTracer::~RayTracer() {
    // delete input_parser;
    // delete scene_parser;
}

void RayTracer::render(Image &output_image, Image &depth_image, Image &normal_image) const {
    output_image.SetAllPixels(scene_parser->getBackgroundColor());
    normal_image.SetAllPixels(BLACK);

    Camera *camera = scene_parser->getCamera();
    Vec3f ambient_color = scene_parser->getAmbientLight();

    int width = output_image.Width(), height = output_image.Height();
    float depth_min = input_parser->depth_min;
    float depth_max = input_parser->depth_max;

    // bottom_left -> (0, 0), upper_right -> (width, height)
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            float u = i * 1.0f / width;
            float v = j * 1.0f / height;

            Hit hit;
            Ray ray = camera->generateRay(Vec2f(u, v));

            // phong model with ray tracing
            Vec3f render_color = traceRay(ray, camera->getTMin(), 0, 1.0f, 1.0f, hit);
            output_image.SetPixel(i, j, render_color);

            float t = hit.getT();
            Vec3f normal = hit.getNormal();

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

Vec3f RayTracer::mirrorDirection(const Vec3f &normal, const Vec3f &incoming) {
    // in-ray is negative and in-ray + out-ray = normal * abs(in-ray * normal)
    Vec3f mirror_dir = incoming - 2 * normal.Dot3(incoming) * normal;
    mirror_dir.Normalize();
    return mirror_dir;
}

bool RayTracer::transmittedDirection(const Vec3f &normal, const Vec3f &incoming, float index_i, float index_t,
                                     Vec3f &transmitted) {
    auto i = incoming;
    i.Negate();
    auto eta = index_i / index_t;
    auto cosThetaTSquare = 1.0f - eta * eta * (1.0f - normal.Dot3(i) * normal.Dot3(i));
    if (cosThetaTSquare < 0.0f) {
        return false;
    }

    transmitted = (eta * (normal.Dot3(i)) - sqrtf(cosThetaTSquare)) * normal - eta * i;
    transmitted.Normalize();
    return true;
}

Vec3f RayTracer::traceRay(Ray &ray, float t_min, int bounces, float weight, float indexOfRefraction, Hit &hit) const {
    Group *group_ptr = scene_parser->getGroup();
    Vec3f ambient_light = scene_parser->getAmbientLight();
    Vec3f background_color = scene_parser->getBackgroundColor();
    int light_num = scene_parser->getNumLights();

    if (bounces > input_parser->max_bounces || weight < input_parser->cutoff_weight) {
        return {0, 0, 0};
    }

    Object3D *object;
    if (input_parser->visualize_grid) {
        object = grid_ptr;
    } else {
        object = group_ptr;
    }

    Vec3f color;
    if (object->intersect(ray, hit, t_min)) {
        Material *material_ptr = hit.getMaterial();
        Vec3f object_color = material_ptr->getDiffuseColor();
        color = ambient_light * object_color;

        Vec3f normal = hit.getNormal();
        Vec3f intersect = hit.getIntersectionPoint();
        Vec3f ray_dir = ray.getDirection();
        // enable back face shading
        bool back_face = normal.Dot3(ray_dir) > 0;
        if (back_face) {
            if (input_parser->shade_back) {
                normal.Negate();
                hit.set(hit.getT(), hit.getMaterial(), normal, ray);
            } else {
                return BLACK;
            }
        }

        if (bounces == 0) {
            float t_stop = input_parser->visualize_grid ? MAXFLOAT : hit.getT();
            RayTree::SetMainSegment(ray, 0, t_stop);
        }

        float epsilon = 1e-5;
        for (int k = 0; k < light_num; k++) {
            Vec3f dir_to_light, light_color;
            float dis_to_light;
            scene_parser->getLight(k)->getIllumination(intersect, dir_to_light, light_color, dis_to_light);

            // generate shadows
            if (input_parser->shadows) {
                Ray ray_shadow(intersect, dir_to_light);
                Hit hit_shadow;
                if (object->intersect(ray_shadow, hit_shadow, epsilon) && dis_to_light > hit_shadow.getT()) {
                    RayTree::AddShadowSegment(ray_shadow, 0, hit_shadow.getT());
                    continue;
                } else {
                    RayTree::AddShadowSegment(ray_shadow, 0, dis_to_light);
                }
            }
            Vec3f add_color = material_ptr->Shade(ray, hit, dir_to_light, light_color);
            color += add_color;
        }

        // generate reflection color
        Vec3f reflected_color = material_ptr->getReflectiveColor();
        if (material_ptr->getReflectiveColor().Length() > epsilon) {
            Vec3f dir_mirror = mirrorDirection(hit.getNormal(), ray.getDirection());
            Ray ray_reflect(intersect, dir_mirror);
            Hit hit_reflect;

            color += reflected_color *
                     traceRay(ray_reflect, epsilon, bounces + 1, weight * reflected_color.Length() / sqrtf(3),
                              indexOfRefraction, hit_reflect);

            if (bounces < input_parser->max_bounces) {
                RayTree::AddReflectedSegment(ray_reflect, 0, hit_reflect.getT());
            }
        }

        // generate refraction color
        Vec3f refracted_color = material_ptr->getTransparentColor();
        if (material_ptr->getTransparentColor().Length() > epsilon) {
            float max_indexOfRefraction = back_face ? 1.0f : material_ptr->getIndexOfRefraction();
            Vec3f dir_transmit;
            bool is_transmitted = transmittedDirection(hit.getNormal(), ray.getDirection(), indexOfRefraction,
                                                       max_indexOfRefraction, dir_transmit);

            if (is_transmitted) {
                Ray ray_refract(intersect, dir_transmit);
                Hit hit_refract;

                color += refracted_color *
                         traceRay(ray_refract, epsilon, bounces + 1, weight * refracted_color.Length() / sqrtf(3),
                                  max_indexOfRefraction, hit_refract);

                if (bounces < input_parser->max_bounces) {
                    RayTree::AddTransmittedSegment(ray_refract, 0, hit_refract.getT());
                }
            }
        }
    } else {
        RayTree::SetMainSegment(ray, 0, MAXFLOAT);
        color = background_color;
    }

    return color;
}



