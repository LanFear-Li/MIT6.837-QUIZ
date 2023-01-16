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
                Vec3f normal = p - c;
                normal.Normalize();
                h.set(t, material_ptr, normal, r);
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
        Vec3f p = ori + dir * t;
        Vec3f n = p - ori;
        n.Normalize();

        h.set(t, material_ptr, n, r);
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
