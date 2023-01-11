#include "object3d.h"

Object3D::Object3D() = default;

Object3D::~Object3D() = default;


Sphere::Sphere(Vec3f c, float r, Material *m) {
    this->center = c;
    this->radius = r;
    this->material_ptr = m;
}

bool Sphere::intersect(const Ray &r, Hit &h, float t_min) {
    Vec3f ori = r.getOrigin(), dir = r.getDirection();

    // let a = ray's origin, b = another point at ray
    // and c = sphere's center, rad = sphere's radius;
    Vec3f a = ori, b = ori + dir * 1.0f;
    Vec3f c = this->center;
    float rad = this->radius;

    // calculate distance from center to ray with dot production
    Vec3f ab = b - a;
    Vec3f ac = c - a;
    float cosine = ab.Dot3(ac) / ab.Length() / ac.Length();
    float sine = sqrt(1 - cosine * cosine);
    float dis_to_ray = ac.Length() * sine;

    // no intersection
    if (dis_to_ray > rad) {
        return false;
    }

    // find max t and check legality
    float unit_length = dir.Length();
    float t_max = (ac.Length() * cosine + this->radius * 10) / unit_length;
    if (t_min > t_max) {
        return false;
    }

    // enumerate all possible t and update hit info
    float step = 0.01;
    for (float t = t_min; t <= t_max; t += step) {
        Vec3f p = ori + dir * t;

        float dis = (p - c).Length();
        if (dis <= rad) {
            if (h.getMaterial() == nullptr || t < h.getT()) {
                h.set(t, material_ptr, r);
            }

            return true;
        }
    }

    return false;
}

Sphere::~Sphere() = default;


Group::Group(int num) {
    this->num_objects = num;
    this->object3D_ptr = new Object3D *[num];
}

bool Group::intersect(const Ray &r, Hit &h, float t_min) {
    bool intersected = false;
    for (int i = 0; i < num_objects; i++) {
        if (object3D_ptr[i]->intersect(r, h, t_min)) {
            intersected = true;
        }
    }

    return intersected;
}

void Group::addObject(int index, Object3D *obj) {
    assert(index >= 0 && index < num_objects);
    object3D_ptr[index] = obj;
}

Group::~Group() {
    /*for (int i = 0; i < num_objects; i++) {
        delete object3D_ptr[i];
    }

    delete object3D_ptr;*/
}
