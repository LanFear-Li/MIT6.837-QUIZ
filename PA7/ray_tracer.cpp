#include "ray_tracer.h"

#include "rayTree.h"
#include "raytracing_stats.h"
#include "film.h"
#include "sampler.h"
#include "filter.h"
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
    } else {
        grid_ptr = nullptr;
    }
}

RayTracer::~RayTracer() {
    // delete input_parser;
    // delete scene_parser;
}

void RayTracer::render(Image &output_image, Image &depth_image, Image &normal_image) const {
    output_image.SetAllPixels(BLACK);
    normal_image.SetAllPixels(BLACK);
    depth_image.SetAllPixels(BLACK);

    Camera *camera = scene_parser->getCamera();
    Vec3f ambient_color = scene_parser->getAmbientLight();

    int width = input_parser->width;
    int height = input_parser->height;
    float depth_min = input_parser->depth_min;
    float depth_max = input_parser->depth_max;

    Film *film;
    Sampler *sampler;
    Filter *filter;

    if (input_parser->sample_type) {
        film = new Film(width, height, input_parser->num_samples);
        sampler = Sampler::getSampler(input_parser->sample_type, input_parser->num_samples);
    } else {
        film = new Film(width, height, 1);
        sampler = new UniformSampler(1);
    }

    if (input_parser->filter_type) {
        filter = Filter::getFilter(input_parser->filter_type, input_parser->filter_param);
    } else {
        filter = new BoxFilter(0.5f);
    }

    // Stats: Before beginning computation
    if (grid_ptr) {
        RayTracingStats::Initialize(width, height, scene_parser->getGroup()->bbox_ptr, grid_ptr->nx, grid_ptr->ny, grid_ptr->nz);
    } else {
        RayTracingStats::Initialize(width, height, scene_parser->getGroup()->bbox_ptr, 0, 0, 0);
    }

    // bottom_left -> (0, 0), upper_right -> (width, height)
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            // pixel margin for regulation
            float size = max(width, height);
            float margin_x = (size - width) / 2.0f;
            float margin_y = (size - height) / 2.0f;
            float u = (i * 1.0f + margin_x) / size;
            float v = (j * 1.0f + margin_y) / size;
            Vec2f pixel(u, v);
            Vec2f pixel_size(1.0f / width, 1.0f / height);

            // super sampling for anti-aliasing
            // without AA, sample one and only, also without filter
            Hit hit;
            for (int k = 0; k < film->getNumSamples(); k++) {
                Vec2f offset = sampler->getSamplePosition(k);
                Ray ray = camera->generateRay(pixel + offset * pixel_size);
                Vec3f color = traceRay(ray, camera->getTMin(), 0, 1.0f, 1.0f, hit);
                film->setSample(i, j, k, offset, color);
            }
            output_image.SetPixel(i, j, filter->getColor(i, j, film));

            // depth_image & normal_image visualization
            float t = hit.getT();
            Vec3f normal = hit.getNormal();

            t = f_clamp(t, depth_min, depth_max);
            t = 1.0f - (t - depth_min) / (depth_max - depth_min);
            Vec3f depth_color(t, t, t);
            depth_image.SetPixel(i, j, depth_color);

            // x, y and z [-1, 1] -> [0, 1]
            float x = fabs(normal.x());
            float y = fabs(normal.y());
            float z = fabs(normal.z());
            Vec3f normal_color(x, y, z);
            normal_image.SetPixel(i, j, normal_color);
        }
    }

    // sample_image & filter_image visualization
    if (input_parser->render_samples) {
        film->renderSamples(input_parser->sample_file, input_parser->sample_zoom_factor);
    }

    if (input_parser->render_filter) {
        film->renderFilter(input_parser->filter_file, input_parser->filter_zoom_factor, filter);
    }

    // Stats: End computation
    if (input_parser->stats) {
        RayTracingStats::PrintStatistics();
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
    if (bounces > input_parser->max_bounces || weight < input_parser->cutoff_weight) {
        return {0, 0, 0};
    }

    // Stats: Non-shadow ray cast
    RayTracingStats::IncrementNumNonShadowRays();

    Group *group_ptr = scene_parser->getGroup();
    Vec3f ambient_light = scene_parser->getAmbientLight();
    Vec3f background_color = scene_parser->getBackgroundColor();
    int light_num = scene_parser->getNumLights();

    Object3D *object;
    // if with grid, using grid intersection for acceleration
    if (input_parser->visualize_grid) {
        object = grid_ptr;
    } else {
        object = group_ptr;
    }

    Vec3f color;
    if (RayCast(object, ray, hit, t_min)) {
        Material *material_ptr = hit.getMaterial();
        Vec3f normal = hit.getNormal();
        Vec3f intersect = hit.getIntersectionPoint();
        Vec3f ray_dir = ray.getDirection();

        Vec3f object_color = material_ptr->getDiffuseColor(intersect);
        color = ambient_light * object_color;

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
                if (RayCast(object, ray_shadow, hit_shadow, epsilon) && dis_to_light > hit_shadow.getT()) {
                    RayTree::AddShadowSegment(ray_shadow, 0, hit_shadow.getT());
                    continue;
                } else {
                    RayTree::AddShadowSegment(ray_shadow, 0, dis_to_light);

                    // Stats: Shadow ray cast
                    RayTracingStats::IncrementNumShadowRays();
                }
            }
            Vec3f add_color = material_ptr->Shade(ray, hit, dir_to_light, light_color);
            color += add_color;
        }

        // generate reflection color
        Vec3f reflected_color = material_ptr->getReflectiveColor(intersect);
        if (material_ptr->getReflectiveColor(intersect).Length() > epsilon) {
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
        Vec3f refracted_color = material_ptr->getTransparentColor(intersect);
        if (material_ptr->getTransparentColor(intersect).Length() > epsilon) {
            float max_indexOfRefraction = back_face ? 1.0f : material_ptr->getIndexOfRefraction(intersect);
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

bool RayTracer::RayCast(Object3D *object, const Ray &r, Hit &h, float t_min) const {
    // no grid or visualize grid, using normal ray cast method(group or grid intersect)
    if (!input_parser->with_grid || input_parser->visualize_grid) {
        return object->intersect(r, h, t_min);
    }

    // otherwise, using fast ray cast method(grid then object intersect)
    return grid_ptr->intersectObject(r, h, t_min);
}



