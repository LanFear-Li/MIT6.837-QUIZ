#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <limits>

#include "ray.h"

class Camera {
public:
    Camera();

    virtual Ray generateRay(Vec2f point) = 0;

    virtual float getTMin() const = 0;

    ~Camera();
};

class OrthographicCamera : public virtual Camera {
public:
    OrthographicCamera(Vec3f c, Vec3f d, Vec3f u, float s);

    Ray generateRay(Vec2f point) override;

    float getTMin() const override;

    ~OrthographicCamera();

    Vec3f center;
    Vec3f direction;
    Vec3f up;
    Vec3f horizontal;
    float size;
};

class PerspectiveCamera : public virtual Camera {
public:
    PerspectiveCamera(Vec3f &center, Vec3f &direction, Vec3f &up, float angle);

    Ray generateRay(Vec2f point) override;

    float getTMin() const override;

    ~PerspectiveCamera();

    Vec3f center;
    Vec3f direction;
    Vec3f up;
    Vec3f horizontal;
    Vec3f fov;
};

#endif