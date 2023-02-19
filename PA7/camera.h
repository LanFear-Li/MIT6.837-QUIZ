#ifndef _PA_CAMERA_H_
#define _PA_CAMERA_H_

#include <limits>
#include <GL/gl.h>
#include <GL/glu.h>

#include "ray.h"
#include "matrix.h"

class Camera {
public:
    Camera();

    virtual void glInit(int w, int h) = 0;

    virtual void glPlaceCamera(void) = 0;

    virtual void dollyCamera(float dist) = 0;

    virtual void truckCamera(float dx, float dy) = 0;

    virtual void rotateCamera(float rx, float ry) = 0;

    virtual Ray generateRay(Vec2f point) = 0;

    virtual float getTMin() const = 0;

    ~Camera();
};

class OrthographicCamera : public virtual Camera {
public:
    OrthographicCamera(const Vec3f& c, const Vec3f& d, const Vec3f& u, float s);

    void glInit(int w, int h) override;

    void glPlaceCamera() override;

    void dollyCamera(float dist) override;

    void truckCamera(float dx, float dy) override;

    void rotateCamera(float rx, float ry) override;

    Ray generateRay(Vec2f point) override;

    float getTMin() const override;

    ~OrthographicCamera();

    Vec3f center;

    Vec3f direction;
    Vec3f up, origin_up;
    Vec3f horizontal;

    // screen plane square size
    float size;
};

class PerspectiveCamera : public virtual Camera {
public:
    PerspectiveCamera(Vec3f &center, Vec3f &direction, Vec3f &up, float fov_angle);

    void glInit(int w, int h) override;

    void glPlaceCamera(void) override;

    void dollyCamera(float dist) override;

    void truckCamera(float dx, float dy) override;

    void rotateCamera(float rx, float ry) override;

    Ray generateRay(Vec2f point) override;

    float getTMin() const override;

    ~PerspectiveCamera();

    // camera center[origin]
    Vec3f center;

    // camera direction basis
    Vec3f direction;
    Vec3f up, origin_up;
    Vec3f horizontal;

    // vertical field of view[FOV]
    float fov;
    float near;
};

#endif