#include "camera.h"

Camera::Camera() = default;

Camera::~Camera() = default;


OrthographicCamera::OrthographicCamera(Vec3f c, Vec3f d, Vec3f u, float s) {
    d.Normalize();

    u.Normalize();
    // TODO: modify up to be orthonormal to the direction

    Vec3f h;
    Vec3f::Cross3(h, d, u);

    this->center = c;
    this->direction = d;
    this->up = u;
    this->horizontal = h;
    this->size = s;
}

Ray OrthographicCamera::generateRay(Vec2f point) {
    point -= Vec2f(0.5f, 0.5f);
    float x = this->center.x() + point.x() * (this->size * this->horizontal).Length();
    float y = this->center.y() + point.y() * (this->size * this->up).Length();

    Vec3f origin(x, y, 0);
    return Ray(origin, this->direction);
}

float OrthographicCamera::getTMin() const {
    return numeric_limits<float>::min();
}

OrthographicCamera::~OrthographicCamera() = default;
