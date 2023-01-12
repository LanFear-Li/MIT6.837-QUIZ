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


PerspectiveCamera::PerspectiveCamera(Vec3f &center, Vec3f &direction, Vec3f &up, float angle) {

}

Ray PerspectiveCamera::generateRay(Vec2f point) {
    return Ray();
}

float PerspectiveCamera::getTMin() const {
    return -10.0f;
}

PerspectiveCamera::~PerspectiveCamera() = default;