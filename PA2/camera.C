#include "camera.h"

Camera::Camera() = default;

Camera::~Camera() = default;


OrthographicCamera::OrthographicCamera(Vec3f c, Vec3f d, Vec3f u, float s) {
    Vec3f h;

    d.Normalize();
    Vec3f::Cross3(h, d, u);
    h.Normalize();
    Vec3f::Cross3(u, h, d);
    u.Normalize();

    this->center = c;
    this->direction = d;
    this->up = u;
    this->horizontal = h;
    this->size = s;
}

Ray OrthographicCamera::generateRay(Vec2f point) {
    point -= Vec2f(0.5f, 0.5f);
    Vec3f origin = this->center;

    origin += this->horizontal * this->size * point.x();
    origin += this->up * this->size * point.y();

    return Ray(origin, this->direction);
}

float OrthographicCamera::getTMin() const {
    return -10.0f;
}

OrthographicCamera::~OrthographicCamera() = default;


PerspectiveCamera::PerspectiveCamera(Vec3f &c, Vec3f &d, Vec3f &u, float fov) {
    Vec3f h;

    d.Normalize();
    Vec3f::Cross3(h, d, u);
    h.Normalize();
    Vec3f::Cross3(u, h, d);
    u.Normalize();

    this->center = c;
    this->direction = d;
    this->up = u;
    this->horizontal = h;

    this->fov = fov;
    this->near = 1.0f / (2 * tan(fov / 2));
}

Ray PerspectiveCamera::generateRay(Vec2f point) {
    point -= Vec2f(0.5f, 0.5f);
    Vec3f screen_loc = this->center + this->direction * this->near + Vec3f(point.x(), point.y(), 0);

    Vec3f origin = this->center;
    Vec3f ray_dir = screen_loc - this->center;
    ray_dir.Normalize();
    return Ray(origin, ray_dir);
}

float PerspectiveCamera::getTMin() const {
    return 0.0f;
}

PerspectiveCamera::~PerspectiveCamera() = default;