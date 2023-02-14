#include "plane.h"

#include "hit.h"
#include "material.h"
#include "raytracing_stats.h"

Plane::Plane() = default;

Plane::Plane(Vec3f &n, float d, Material *m) {
    normal = n;
    distance = d;
    material_ptr = m;

    object_type = PLANE;

    // plane has infinite bbox
    bbox_ptr = nullptr;
}

Plane::Plane(Vec3f &a, Vec3f &b, Vec3f &c, Material *m) {
    Vec3f::Cross3(normal, b - a, c - b);
    normal.Normalize();
    distance = a.Dot3(normal);
    material_ptr = m;

    object_type = PLANE;

    // plane has infinite bbox
    bbox_ptr = nullptr;
}

bool Plane::intersect(const Ray &r, Hit &h, float t_min) {
    // Stats: Ray-Primitive intersection operation
    RayTracingStats::IncrementNumIntersections();

    Vec3f ori = r.getOrigin(), dir = r.getDirection();

    // ray parallel with plane
    float epsilon = 1e-5;
    if (fabsf(dir.Dot3(normal)) < epsilon) {
        return false;
    }

    // (origin + t * direction) * normal = distance
    float t = (distance - ori.Dot3(normal)) / (dir.Dot3(normal));

    if (t < t_min) {
        return false;
    }

    h.set(t, material_ptr, normal, r);
    return true;
}

void Plane::paint() {
    // calculate plane basis vector
    Vec3f v(1, 0, 0);
    float epsilon = 1e-5;
    if (fabs(this->normal.y()) < epsilon && fabs(this->normal.z()) < epsilon) {
        v.Set(0, 1, 0);
    }

    Vec3f basis_x, basis_y;
    Vec3f::Cross3(basis_x, v, this->normal);
    Vec3f::Cross3(basis_y, this->normal, basis_x);

    float MAX_LIMIT = 1e6;
    Vec3f a = basis_x * MAX_LIMIT + this->normal * distance;
    Vec3f b = basis_y * MAX_LIMIT + this->normal * distance;
    Vec3f c = basis_x * -MAX_LIMIT + this->normal * distance;
    Vec3f d = basis_y * -MAX_LIMIT + this->normal * distance;

    this->material_ptr->glSetMaterial();
    glBegin(GL_QUADS);
    glNormal3f(this->normal.x(), this->normal.y(), this->normal.z());
    glVertex3f(a.x(), a.y(), a.z());
    glVertex3f(b.x(), b.y(), b.z());
    glVertex3f(c.x(), c.y(), c.z());
    glVertex3f(d.x(), d.y(), d.z());
    glEnd();
}

void Plane::insertIntoGrid(Grid *g, Matrix *m) {

}