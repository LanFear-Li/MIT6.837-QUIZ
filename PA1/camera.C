#include "camera.h"

Camera::Camera() = default;

Camera::~Camera() = default;


OrthographicCamera::OrthographicCamera(Vec3f c, Vec3f d, Vec3f u, float s) {
    this->center = c;
    this->direction = d;
    this->up = u;
    this->size = s;
}

Ray OrthographicCamera::generateRay(Vec2f point) {
    return Ray();
}

float OrthographicCamera::getTMin() const {
    return 0;
}

OrthographicCamera::~OrthographicCamera() = default;
