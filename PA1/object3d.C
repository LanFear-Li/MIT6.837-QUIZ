#include "object3d.h"

Object3D::Object3D() = default;

Object3D::~Object3D() = default;


Sphere::Sphere(Vec3f c, float r, Material *m) {
    this->center = c;
    this->radius = r;
    this->material_ptr = m;
}

bool Sphere::intersect(const Ray &r, Hit &h, float min) {
    return false;
}

Sphere::~Sphere() = default;


Group::Group(int num) {
    this->num_objects = num;
}

bool Group::intersect(const Ray &r, Hit &h, float min) {
    return false;
}

void Group::addObject(int index, Object3D *obj) {

}

Group::~Group() = default;
