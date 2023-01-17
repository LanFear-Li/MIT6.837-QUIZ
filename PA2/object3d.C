#include "object3d.h"

Object3D::Object3D() = default;

Object3D::~Object3D() = default;


Sphere::Sphere(Vec3f c, float r, Material *m) {
    this->center = c;
    this->radius = r;
    this->material_ptr = m;
}

bool Sphere::intersect(const Ray &r, Hit &h, float t_min) {
    Vec3f cen = this->center;
    float rad = this->radius;
    Vec3f ori = r.getOrigin(), dir = r.getDirection();
    Vec3f start = ori - cen;

    // calculate ray-sphere intersection with equation:
    // (start + t * dir) * (start + t * dir) - rad * rad = 0
    // (dir * dir) * t^2 + (2 * start * dir) * t + (start * start - rad * rad) = 0
    float a = 1;
    float half_b = start.Dot3(dir);
    float c = start.Dot3(start) - rad * rad;

    // discriminant = 4(half_b * half_b - a * c)
    float discriminant = half_b * half_b - a * c;
    if (discriminant < 0) {
        return false;
    }

    // get equation root
    float sqrt_dis = sqrt(discriminant);
    float t = (-half_b - sqrt_dis) / a;
    if (t < t_min) {
        t = (-half_b + sqrt_dis) / a;
        if (t < t_min) {
            return false;
        }
    }

    if (h.getMaterial() == nullptr || t < h.getT()) {
        Vec3f p = ori + dir * t;
        Vec3f normal = p - cen;
        normal.Normalize();

        h.set(t, material_ptr, normal, r);
    }

    return true;
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


Plane::Plane(Vec3f &normal, float d, Material *m) {
    this->normal = normal;
    this->distance = d;
    this->material_ptr = m;
}

bool Plane::intersect(const Ray &r, Hit &h, float t_min) {
    Vec3f ori = r.getOrigin(), dir = r.getDirection();

    // ray parallel with plane
    if (dir.Dot3(this->normal) == 0) {
        return false;
    }

    // (origin + t * direction) * normal = distance
    float t = (this->distance - ori.Dot3(this->normal)) / (dir.Dot3(this->normal));

    if (t < t_min) {
        return false;
    }

    if (h.getMaterial() == nullptr || t < h.getT()) {
        h.set(t, material_ptr, this->normal, r);
    }

    return true;
}

Plane::~Plane() = default;


Triangle::Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m) {
    this->a = a;
    this->b = b;
    this->c = c;

    this->material_ptr = m;
}

bool Triangle::intersect(const Ray &r, Hit &h, float t_min) {
    Vec3f ori = r.getOrigin(), dir = r.getDirection();

    Vec3f normal;
    Vec3f::Cross3(normal, this->a - this->b, this->a - this->c);
    normal.Normalize();
    float dis = this->a.Dot3(normal);

    // ray parallel with plane
    if (dir.Dot3(normal) == 0) {
        return false;
    }

    // (origin + t * direction) * normal = distance
    float t = (dis - ori.Dot3(normal)) / (dir.Dot3(normal));

    if (t < t_min) {
        return false;
    }

    // get intersect point with the plane formed by triangle
    Vec3f p = ori + dir * t;

    // use cross product to check whether inside the triangle
    Vec3f ab = this->b - this->a, bc = this->c - this->b, ca = this->a - this->c;
    Vec3f ap = p - this->a, bp = p - this->b, cp = p - this->c;

    Vec3f cross;
    Vec3f::Cross3(cross, ab, ap);
    float cross_a = normal.Dot3(cross);
    Vec3f::Cross3(cross, bc, bp);
    float cross_b = normal.Dot3(cross);
    Vec3f::Cross3(cross, ca, cp);
    float cross_c = normal.Dot3(cross);

    // TODO: ray-triangle intersection with barycentric coordinate
    if (cross_a > 0 && cross_b > 0 && cross_c > 0 || cross_a <= 0 && cross_b <= 0 && cross_c <= 0) {
        if (h.getMaterial() == nullptr || t < h.getT()) {
            h.set(t, material_ptr, normal, r);
        }

        return true;
    }

    return false;
}

Triangle::~Triangle() = default;


Transform::Transform(Matrix &m, Object3D *o) {
    this->mat = m;
    this->object3d_ptr = o;
}

bool Transform::intersect(const Ray &r, Hit &h, float t_min) {
    // TODO: transform the ray
    return object3d_ptr->intersect(r, h, t_min);
}

Transform::~Transform() = default;
